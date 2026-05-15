# Flash Layout

This project keeps the TI BIM and CCFG area intact and places two application
images in separate flash slots.

| Area | Start | End | Purpose |
| --- | ---: | ---: | --- |
| User image | `0x00000000` | `0x0002FFFF` | Preferred image checked by BIM |
| Persistent fallback | `0x00030000` | `0x00051FFF` | Fallback image if the user image is absent or invalid |
| Metadata | `0x00052000` | `0x00053FFF` | Reserved update metadata area |
| Recovery | `0x00054000` | `0x00055FFF` | Reserved recovery area |
| BIM + CCFG | `0x00056000` | `0x00057FFF` | Existing TI BIM and device configuration |

Each application slot starts with a TI OAD image header. The reset vector table
is placed at offset `0x100` from the slot base.

| Image | Header | Entry |
| --- | ---: | ---: |
| `new-firmware` | `0x00000000` | `0x00000100` |
| `old-firmware` | `0x00030000` | `0x00030100` |
