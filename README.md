# BIL304 HW3 Contiki-NG Firmware Pair

## Purpose

This project provides a pair of firmware images for analyzing the transition between two versions of an application. It is designed to be used with the "Rehydrator" analysis tool for the BIL304 Operating Systems course.

## Folder Location

Copy or mount this project folder under your Contiki-NG tree. For consistency with the Makefile configuration, it is recommended to place it under `examples/bil304-hw3`:

```sh
cp -r rehydrator /path/to/contiki-ng/examples/bil304-hw3
cd /path/to/contiki-ng/examples/bil304-hw3
```

In the `Makefile`, the path to the Contiki-NG root directory should match your directory depth:

```makefile
CONTIKI ?= ../..
```

## Firmware Roles

It builds two separate firmware images:

- **old-firmware**: The primary firmware that will be flashed to the CC1352R device and will run after reset.
- **new-firmware**: The newer firmware image that will be analyzed and stored as data in the staging area.

## Build For CC1352R

First, list supported boards to ensure the correct name:

```sh
make TARGET=simplelink boards
```

Build both images for the CC1352R1 SensorTag:

```sh
make clean
make TARGET=simplelink BOARD=sensortag/cc1352r1 old-firmware new-firmware
```

## Build For Z1/Cooja

For simulations using the Z1 platform:

```sh
make TARGET=z1 old-firmware new-firmware
```

## Package New Firmware For Staging

The hardware adaptation code expects the new firmware storage area to start with a `FwImageHeader`, followed by the new firmware payload. After building `new-firmware`, create the container:

```sh
python3 tools/package_cc1352r_image.py new-firmware --version 0x00020000
```

This produces:

- `new-firmware.container.bin`
- `new-firmware.container.hex`

## Flashing Strategy

### Flashing To CC1352R

The device must first be programmed with the old firmware image. Create the HEX file from the build output:

```sh
arm-none-eabi-objcopy -O ihex \
  build/simplelink/sensortag/cc1352r1/old-firmware.simplelink \
  build/simplelink/sensortag/cc1352r1/old-firmware.hex
```

Load `old-firmware.hex` as the active application using UniFlash.

### Loading Staged Firmware

The new firmware must **not** be loaded as the active application image. It must be loaded into the staging/data area, starting at `0x00030000`, using the generated `new-firmware.container.hex`.

> [!IMPORTANT]
> When loading `new-firmware.container.hex`, do not perform a full chip erase. It must be programmed only into the staging area. Otherwise, the active old firmware or CCFG area may be erased.

## ELF Analysis

### For CC1352R/Arm firmware:

```sh
cd build/simplelink/sensortag/cc1352r1

file new-firmware.simplelink
arm-none-eabi-readelf -h new-firmware.simplelink
arm-none-eabi-readelf -S new-firmware.simplelink
arm-none-eabi-readelf -l new-firmware.simplelink
arm-none-eabi-readelf -s new-firmware.simplelink
arm-none-eabi-objdump -h new-firmware.simplelink
arm-none-eabi-objdump -d new-firmware.simplelink
arm-none-eabi-nm -n new-firmware.simplelink
arm-none-eabi-size new-firmware.simplelink
arm-none-eabi-strings new-firmware.simplelink
```

### For Z1/MSP430 firmware:

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

In your report, explain the ELF class, target architecture, entry address, section meanings, code/data sizes, and important symbols.

## Important Safety Notes

- **Do not flash `new-firmware.hex`** as the active application image.
- The active image is **`old-firmware.hex`**.
- The new firmware must be stored as data in the staging area (`0x00030000`).
- **Do not mass erase** the device when programming the staging HEX file.
- The CCFG sector must not be erased or overwritten.
- The staged new firmware will not boot automatically without BIM/bootloader support.
