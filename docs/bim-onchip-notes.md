# BIM On-Chip Notes

The bootloader is kept as a separate TI BIM project under `../bim_onchip/`.
This repository adapts the firmware images to the flash layout expected by that
bootloader.

Important BIM facts for this project:

- BIM starts at `0x00056000`.
- CCFG is stored at the end of flash and must not be erased accidentally.
- Page 0 user image is checked before the persistent fallback image.
- The debug unsecure BIM flow can accept OAD headers where CRC and image length
  fields are left as `0xFFFFFFFF`.

The firmware linker scripts discard `.ccfg` so device configuration comes from
the BIM HEX file, not from the application images.
