| Section | `old-firmware` VMA / boyut | `new-firmware` VMA / boyut | Yorum |
|---|---:|---:|---|
| `.image_header` | `0x00030000` / `0x38` | `0x00000000` / `0x38` | OAD header slot başlangıcında yer alır. |
| `.resetVecs` | `0x00030100` / `0x40` | `0x00000100` / `0x40` | Reset vector tablosu header'dan sonra başlar. |
| `.text` | `0x00030140` / `0x11b60` | `0x00000140` / `0x119d4` | Uygulama kodu kendi flash slotunda kalır. |
| `.ARM.exidx` | `0x00041ca0` / `0x8` | `0x00011b14` / `0x8` | Exception index flash içindedir. |
| `.data` | `0x20001b20` / `0x4d4` | `0x20001b20` / `0x4d4` | Çalışma sırasında SRAM'e kopyalanacak initialized data. |
| `vtable_ram` | `0x20002000` / `0xd8` | `0x20002000` / `0xd8` | RAM vektör tablosu alanı. |
| `.bss` | `0x200020d8` / `0x3234` | `0x200020d8` / `0x3234` | SRAM'de sıfırlanacak veri. |
| `.stack` | `0x2000530c` / `0x604` | `0x2000530c` / `0x604` | Stack alanı. |

