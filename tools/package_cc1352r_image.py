#!/usr/bin/env python3
import argparse
import pathlib
import struct
import sys
import zlib

FW_IMAGE_MAGIC = 0x46573352
FW_LAYOUT_VERSION = 1
FW_APP_BASE = 0x00000000
FW_NEW_IMAGE_BASE = 0x00030000
FW_APP_SIZE = 0x00030000
FW_NEW_IMAGE_SIZE = 0x00022000

HEADER = struct.Struct("<IHHIIIII4II")
FW_IMAGE_MAX_PAYLOAD_SIZE = FW_NEW_IMAGE_SIZE - HEADER.size


def crc32(data):
    return zlib.crc32(data) & 0xFFFFFFFF


def parse_u32(value):
    return int(value, 0) & 0xFFFFFFFF


def intel_hex_record(address, record_type, data):
    body = bytearray()
    body.append(len(data))
    body.extend(struct.pack(">H", address & 0xFFFF))
    body.append(record_type)
    body.extend(data)
    checksum = ((~sum(body) + 1) & 0xFF)
    return ":" + body.hex().upper() + f"{checksum:02X}"


def write_ihex(path, base_address, data):
    lines = []
    current_upper = None

    for offset in range(0, len(data), 16):
        absolute = base_address + offset
        upper = (absolute >> 16) & 0xFFFF
        if upper != current_upper:
            lines.append(intel_hex_record(0, 0x04, struct.pack(">H", upper)))
            current_upper = upper
        lines.append(intel_hex_record(absolute & 0xFFFF, 0x00, data[offset:offset + 16]))

    lines.append(intel_hex_record(0, 0x01, b""))
    path.write_text("\n".join(lines) + "\n", encoding="ascii")


def trim_payload(data):
    end = len(data)
    while end > 0 and data[end - 1] in (0x00, 0xFF):
        end -= 1
    return data[:end]


def warn(message):
    print(f"warning: {message}", file=sys.stderr)


def load_payload(path, source_limit):
    raw = path.read_bytes()
    if len(raw) > source_limit:
        warn(
            f"input is {len(raw)} bytes; ignoring bytes after 0x{source_limit:X} "
            "so data outside the active application area, including CCFG, is not packaged"
        )
    return raw[:source_limit]


def validate_args(source_limit, max_size, target):
    if source_limit > FW_APP_SIZE:
        raise SystemExit(
            f"source-limit 0x{source_limit:X} crosses the active app boundary "
            f"0x{FW_APP_SIZE:X}; refusing to risk packaging reserved flash/CCFG data"
        )

    if max_size > FW_IMAGE_MAX_PAYLOAD_SIZE:
        raise SystemExit(
            f"max-size {max_size} exceeds staging payload capacity "
            f"{FW_IMAGE_MAX_PAYLOAD_SIZE}"
        )

    if target != FW_APP_BASE:
        raise SystemExit(
            f"target must be 0x{FW_APP_BASE:08X}; this image is copied to the "
            "active app region before execution, not executed from staging"
        )


def validate_container(base, container_size):
    staging_end = FW_NEW_IMAGE_BASE + FW_NEW_IMAGE_SIZE
    container_end = base + container_size

    if base < FW_NEW_IMAGE_BASE or container_end > staging_end:
        raise SystemExit(
            f"container range 0x{base:08X}-0x{container_end - 1:08X} is outside "
            f"staging range 0x{FW_NEW_IMAGE_BASE:08X}-0x{staging_end - 1:08X}"
        )

    if base != FW_NEW_IMAGE_BASE:
        warn(
            f"output base 0x{base:08X} differs from HW3_NEW_IMAGE_BASE "
            f"0x{FW_NEW_IMAGE_BASE:08X}; old-firmware will only check the layout base"
        )


def main():
    parser = argparse.ArgumentParser(
        description="Wrap a CC1352R firmware binary with the homework image header."
    )
    parser.add_argument("payload", type=pathlib.Path, help="new firmware .bin payload")
    parser.add_argument("--version", default="0x00020000", help="image version")
    parser.add_argument("--target", default="0x00000000", help="target app address")
    parser.add_argument("--base", default="0x00030000", help="flash address for output HEX")
    parser.add_argument("--source-limit", default="0x00030000",
                        help="read only this many bytes from the raw payload")
    parser.add_argument("--max-size", default=f"0x{FW_IMAGE_MAX_PAYLOAD_SIZE:08X}",
                        help="maximum payload bytes after optional trimming and padding")
    parser.add_argument("--trim-padding", action="store_true",
                        help="trim trailing 0x00/0xFF padding from a sparse raw dump")
    parser.add_argument("--no-trim", action="store_true",
                        help=argparse.SUPPRESS)
    parser.add_argument("--out-bin", type=pathlib.Path, help="container binary output")
    parser.add_argument("--out-hex", type=pathlib.Path, help="addressed Intel HEX output")
    args = parser.parse_args()

    source_limit = parse_u32(args.source_limit)
    max_size = parse_u32(args.max_size)
    target = parse_u32(args.target)
    validate_args(source_limit, max_size, target)

    payload = load_payload(args.payload, source_limit)
    if args.trim_padding:
        payload = trim_payload(payload)

    padding = (-len(payload)) % 4
    if padding:
        payload += b"\xFF" * padding

    if len(payload) > max_size:
        raise SystemExit(
            f"payload is {len(payload)} bytes, max is {max_size}; "
            "reduce firmware size or increase the storage area"
        )

    version = parse_u32(args.version)
    base = parse_u32(args.base)

    image_crc = crc32(payload)
    header_without_crc = HEADER.pack(
        FW_IMAGE_MAGIC,
        HEADER.size,
        FW_LAYOUT_VERSION,
        version,
        len(payload),
        image_crc,
        target,
        0,
        0, 0, 0, 0,
        0,
    )
    header_crc = crc32(header_without_crc)
    header = bytearray(header_without_crc)
    struct.pack_into("<I", header, HEADER.size - 4, header_crc)

    container = bytes(header) + payload
    validate_container(base, len(container))

    out_bin = args.out_bin or args.payload.with_suffix(".container.bin")
    out_hex = args.out_hex or args.payload.with_suffix(".container.hex")

    out_bin.write_bytes(container)
    write_ihex(out_hex, base, container)

    print(f"payload:   {args.payload}")
    print(f"out bin:   {out_bin}")
    print(f"out hex:   {out_hex}")
    print(f"base:      0x{base:08X}")
    print(f"version:   0x{version:08X}")
    print(f"size:      {len(payload)} bytes")
    print(f"crc32:     0x{image_crc:08X}")
    print(f"header:    {HEADER.size} bytes")
    print(f"headercrc: 0x{header_crc:08X}")


if __name__ == "__main__":
    main()
