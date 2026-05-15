#!/usr/bin/env sh
set -eu

TARGET="${TARGET:-simplelink}"
BOARD="${BOARD:-sensortag/cc1352r1}"
BUILD_ROOT="${BUILD_ROOT:-build/$TARGET/$BOARD}"
READELF="${READELF:-arm-none-eabi-readelf}"

find_elf() {
  image="$1"
  find "$BUILD_ROOT" -type f -name "$image.$TARGET" | head -n 1
}

section_addr() {
  elf="$1"
  section="$2"
  "$READELF" -SW "$elf" | awk -v section="$section" '$3 == section { print "0x" $5; exit }'
}

check_section() {
  image="$1"
  elf="$2"
  section="$3"
  expected="$4"
  actual="$(section_addr "$elf" "$section")"

  if [ "$actual" != "$expected" ]; then
    echo "$image $section expected $expected, got ${actual:-missing}" >&2
    exit 1
  fi

  echo "$image $section = $actual"
}

old_elf="$(find_elf old-firmware)"
new_elf="$(find_elf new-firmware)"

if [ -z "$old_elf" ] || [ -z "$new_elf" ]; then
  echo "Could not find both firmware ELF files under $BUILD_ROOT" >&2
  echo "Run: make TARGET=$TARGET BOARD=$BOARD upload-files" >&2
  exit 1
fi

check_section old-firmware "$old_elf" .image_header 0x00030000
check_section old-firmware "$old_elf" .resetVecs 0x00030100
check_section new-firmware "$new_elf" .image_header 0x00000000
check_section new-firmware "$new_elf" .resetVecs 0x00000100
