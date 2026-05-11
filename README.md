# BIL304 HW3 Contiki-NG Firmware Pair

This folder is the Docker/Contiki-NG side of the homework. Copy or mount this
project folder under `contiki-ng/examples/`, for example:

```sh
cp -r empty /path/to/contiki-ng/examples/bil304-hw3
cd /path/to/contiki-ng/examples/bil304-hw3
```

It builds two separate firmware images:

- `old-firmware`: the firmware assumed to be already running on the device.
- `new-firmware`: the newer firmware image that will be analyzed and stored as data.

## Build In Docker

For the Cooja/Z1 part:

```sh
make TARGET=z1 old-firmware.z1 new-firmware.z1
```

For CC1352R1 LaunchPad with Contiki-NG's SimpleLink platform:

```sh
make clean
make TARGET=simplelink BOARD=launchpad/cc1352r1 old-firmware.bin new-firmware.bin
```

If your Contiki-NG tree uses a different board name, list supported boards with:

```sh
make TARGET=simplelink boards
```

## Package The New CC1352R Image

The hardware adaptation code expects the new firmware storage area to start with a
`FwImageHeader`, followed by the new firmware payload. After building
`new-firmware.bin`, create the container:

```sh
python3 tools/package_cc1352r_image.py new-firmware.bin --version 0x00020000
```

This produces:

- `new-firmware.container.bin`
- `new-firmware.container.hex`

The HEX file is addressed at `0x00030000`, so it is the convenient file to load into
the "new firmware storage" area with UniFlash while keeping the old firmware in the
active app area.

By default, the packaging script keeps only the active-application address range
from the raw binary, trims trailing `0x00`/`0xFF` padding, adds 4-byte alignment,
and checks that the result fits in the 136 KiB new-image storage area.

## ELF Analysis Commands

For Z1/MSP430 firmware:

```sh
file new-firmware.z1
msp430-readelf -h new-firmware.z1
msp430-readelf -S new-firmware.z1
msp430-readelf -l new-firmware.z1
msp430-readelf -s new-firmware.z1
msp430-objdump -h new-firmware.z1
msp430-objdump -d new-firmware.z1
msp430-nm -n new-firmware.z1
msp430-size new-firmware.z1
msp430-strings new-firmware.z1
```

For CC1352R/Arm firmware:

```sh
file new-firmware.elf
arm-none-eabi-readelf -h new-firmware.elf
arm-none-eabi-readelf -S new-firmware.elf
arm-none-eabi-readelf -l new-firmware.elf
arm-none-eabi-readelf -s new-firmware.elf
arm-none-eabi-objdump -h new-firmware.elf
arm-none-eabi-objdump -d new-firmware.elf
arm-none-eabi-nm -n new-firmware.elf
arm-none-eabi-size new-firmware.elf
arm-none-eabi-strings new-firmware.elf
```

In the report, do not paste only command output. Explain the ELF class, target
architecture, entry address, section meanings, code/data sizes, important symbols,
and why the file is an executable ELF rather than a raw binary.
