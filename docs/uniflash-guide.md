# UniFlash Guide

Use Intel HEX files when possible because address information is already encoded
in the file.

Recommended flashing order:

1. Flash `../bim_onchip/Debug_unsecure/bim_onchip.hex`.
2. Flash `upload/old-firmware.hex`.
3. Reset the board and confirm the persistent firmware runs.
4. Flash `upload/new-firmware.hex`.
5. Reset the board and confirm the new firmware runs.

Do not use mass erase for normal firmware update testing. The BIM and CCFG area
at `0x00056000 - 0x00057FFF` must remain valid.

If BIN files are used instead of HEX files, set the start address manually:

| File | Start address |
| --- | ---: |
| `upload/new-firmware.bin` | `0x00000000` |
| `upload/old-firmware.bin` | `0x00030000` |
