<div align="center">

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&duration=3000&pause=800&color=00ADD8&center=true&vCenter=true&width=850&lines=CC1352R+TI+BIM%2FOAD+Yerlesimi;BIM+Degismeden+Firmware+Duzeni;User+Imaj+Page+0;Persistent+Fallback+0x30000;BIL304+HW3+3.+Asama" />

<h1>BIL304 HW3 - CC1352R TI BIM/OAD Firmware Yerlesimi</h1>

<p>
  <img src="https://img.shields.io/badge/Hedef-CC1352R-00ADD8" />
  <img src="https://img.shields.io/badge/OS-Contiki--NG-success" />
  <img src="https://img.shields.io/badge/Boot-TI%20BIM%2FOAD-orange" />
  <img src="https://img.shields.io/badge/BIM-Degismeden-blue" />
  <img src="https://img.shields.io/badge/Kontrol-ELF%20%2B%20Section-8A2BE2" />
</p>

<p>
  <b>CC1352R uzerinde mevcut TI BIM dosyasini degistirmeden user firmware,
  persistent fallback firmware ve CCFG/BIM yerlesimini aciklayan calisma.</b>
</p>

<p>
  User image page 0 | Persistent fallback 0x30000 | OAD header |
  CCFG guvenligi | ELF/section kontrolu
</p>

<p>
  <a href="#projedeki-dosyalar">Dosyalar</a> |
  <a href="#flash-yerlesimi">Flash Yerlesimi</a> |
  <a href="#boot-akisi">Boot Akisi</a> |
  <a href="#derleme">Derleme</a> |
  <a href="#kontrol-komutlari">Kontrol</a>
</p>

</div>

Bu repo CC1352R uzerinde **TI BIM/OAD** akisiyla iki uygulama imaji
yerlestirmek icin hazirlandi. Bu versiyonun temel karari sudur:

```text
BIM dosyasi degistirilmeyecek.
Firmware layout mevcut BIM'in arama sirasina uydurulacak.
```

Mevcut Debug_unsecure BIM once page 0'da `APPSTACKLIB` user imaji arar.
User imaj bulunamazsa page 1'den itibaren `PERSISTENT_APP` fallback imaji
tarar. Bu yuzden user imaj page 0'a, persistent fallback imaj `0x00030000`
slotuna yerlestirilir.

## Projedeki Dosyalar

| Dosya | Gorev |
| --- | --- |
| `new-firmware.c` | User uygulama. BIM'in ilk aradigi `APPSTACKLIB` imajidir. |
| `old-firmware.c` | Persistent/fallback uygulama. User imaj gecersizse BIM bunu secebilir. |
| `oad_layout.h` | Flash slotlari, OAD header boyutu, entry adresleri ve firmware versiyonlari. |
| `oad_image_header.h` | Docker/Contiki build icin gereken TI OAD header sabitleri ve struct tanimlari. |
| `oad_hdr.c` | User imaj icin TI OAD image header. |
| `oad_hdr_old.c` | Persistent fallback imaj icin TI OAD image header. |
| `new-firmware.ld` | User imaji `0x00000000` slotuna linkler. |
| `old-firmware.ld` | Persistent fallback imaji `0x00030000` slotuna linkler. |
| `Makefile` | Contiki projelerini ve ilgili OAD header kaynaklarini build'e ekler. |
| `project-conf.h` | Contiki log seviyesini ayarlar. |

## Flash Yerlesimi

CC1352R program flash alani `352 KiB` (`0x00058000`) kabul edilir. Sayfa
boyutu `8 KiB` (`0x2000`).

| Alan | Baslangic | Bitis | Boyut | Gorev |
| --- | ---: | ---: | ---: | --- |
| User image | `0x00000000` | `0x0002FFFF` | 192 KiB | BIM'in ilk aradigi `APPSTACKLIB` imaj |
| Persistent fallback image | `0x00030000` | `0x00051FFF` | 136 KiB | User imaj yoksa/gecersizse fallback |
| Update metadata | `0x00052000` | `0x00053FFF` | 8 KiB | Ayrilmis alan |
| Recovery/reserved | `0x00054000` | `0x00055FFF` | 8 KiB | Ayrilmis alan |
| BIM + CCFG | `0x00056000` | `0x00057FFF` | 8 KiB | TI BIM ve CCFG |

Toplam:

```text
192 KiB + 136 KiB + 8 KiB + 8 KiB + 8 KiB = 352 KiB
```

## OAD Header ve Entry

Her uygulama slotunun ilk `0x100` byte'i TI OAD image header icin ayrilir.
Reset vektorleri bu header alanindan sonra baslar.

User imaj:

```text
0x00000000  TI OAD image header
0x00000100  reset vectors / prgEntry
```

Persistent fallback imaj:

```text
0x00030000  TI OAD image header
0x00030100  reset vectors / prgEntry
```

`oad_layout.h` icindeki ilgili sabitler:

```c
#define OAD_USER_IMAGE_BASE         0x00000000UL
#define OAD_PERSISTENT_IMAGE_BASE   0x00030000UL
#define OAD_IMAGE_HEADER_SIZE       0x00000100UL
#define OAD_USER_ENTRY              (OAD_USER_IMAGE_BASE + OAD_IMAGE_HEADER_SIZE)
#define OAD_PERSISTENT_ENTRY        (OAD_PERSISTENT_IMAGE_BASE + OAD_IMAGE_HEADER_SIZE)
```

## OAD Header Icerigi

User imaj header'i:

```text
imgType   = OAD_IMG_TYPE_APPSTACKLIB
startAddr = 0x00000000
prgEntry  = 0x00000100
softVer   = 0x00020000
```

Persistent fallback imaj header'i:

```text
imgType   = OAD_IMG_TYPE_PERSISTENT_APP
startAddr = 0x00030000
prgEntry  = 0x00030100
softVer   = 0x00010000
```

Header yapilari repo icindeki TI OAD uyumlu dosyadan gelir:

```c
#include "oad_image_header.h"
```

## Boot Akisi

Bu repo mevcut BIM dosyasini degistirmeyen akisa gore duzenlenmistir.

```text
Reset
  -> ROM/CCFG ayarlari BIM bolgesine gider
  -> BIM page 0'da APPSTACKLIB user imaji arar
  -> Gecerli user imaj varsa 0x00000100 adresine atlar
  -> User imaj yoksa/gecersizse page 1'den itibaren PERSISTENT_APP arar
  -> Persistent fallback imaj gecerse 0x00030100 adresine atlar
```

Beklenen LED davranisi:

- user imaj calisiyorsa kirmizi LED heartbeat,
- persistent fallback imaj calisiyorsa yesil LED heartbeat.

## Derleme

Contiki ortamini Docker icinde kullanirken iki imaj ayri derlenmelidir. Bunun
sebebi Contiki-NG `LDSCRIPT` degiskenini proje bazli degil global kullanmasidir.

User imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
```

Persistent fallback imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
```

Docker icinde `CONTIKI` yolu otomatik degilse:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 CONTIKI=/path/to/contiki-ng LDSCRIPT=new-firmware.ld new-firmware
make TARGET=simplelink BOARD=sensortag/cc1352r1 CONTIKI=/path/to/contiki-ng LDSCRIPT=old-firmware.ld old-firmware
```

Beklenen ELF dosyalari:

```text
build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
```

## OAD Image Tool

`oad_hdr.c` ve `oad_hdr_old.c` icindeki `crc32`, `len`, `imgEndAddr` ve segment
length alanlari build oncesinde placeholder durumdadir. Debug BIM denemesinde
bu kabul edilebilir. Release veya secure akista TI OAD Image Tool bu alanlari
doldurmalidir.

Aracin parametreleri SDK surumune gore degisebildigi icin build ortaminda once
yardim ciktisi kontrol edilmelidir:

```sh
oad_image_tool --help
```

## Cihaza Yukleme Sirasi

Bolge bazli programlama yapilmalidir. Full chip erase, son sektordeki BIM/CCFG
alanini silebilir.

1. BIM imaji:

```text
0x00056000 - 0x00057FFF
```

2. User imaj:

```text
0x00000000 - 0x0002FFFF
```

3. Persistent fallback imaj:

```text
0x00030000 - 0x00051FFF
```

## Kontrol Komutlari

ELF icinde header ve entry adreslerini kontrol etmek icin:

```sh
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
```

Beklenen:

```text
new-firmware: .image_header 0x00000000, .resetVecs 0x00000100
old-firmware: .image_header 0x00030000, .resetVecs 0x00030100
```

Header icerigini kontrol etmek icin:

```sh
arm-none-eabi-objdump -s -j .image_header build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
arm-none-eabi-objdump -s -j .image_header build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
```

Beklenen magic:

```text
43 43 31 33 78 32 52 31 = CC13x2R1
```

Kontrol edilmesi gerekenler:

- `.image_header` user imajda `0x00000000`,
- `.resetVecs` user imajda `0x00000100`,
- `.image_header` persistent fallback imajda `0x00030000`,
- `.resetVecs` persistent fallback imajda `0x00030100`,
- uygulama imajlari `0x00056000 - 0x00057FFF` BIM/CCFG alanina tasmaz.

## Notlar

- Bu repo TI BIM kaynak projesini icermez.
- Bu layout, mevcut BIM dosyasini degistirmeme kararina gore secilmistir.
- Docker/Contiki build ortami repo icinde degildir.
- Release/secure OAD icin CRC/signature/post-build adimlari TI OAD Image Tool
  ile tamamlanmalidir.
- Flash yuklerken BIM/CCFG son sektoru korunmalidir.

## TI Kaynaklari

TI OAD image header:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_sdk/1.60.00.29_new/exports/docs/ti154stack/html/oad/image-header.html

TI OAD Image Tool:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_26x2_sdk/4.10.00.78/exports/docs/proprietary-rf/proprietary-rf-users-guide/oad/tools.html

TI OAD linker duzeni:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_26x2_sdk/4.40.04.04/exports/docs/thread/html/thread-oad/create-tiop-oad-image.html
