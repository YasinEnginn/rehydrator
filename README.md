<div align="center">

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&duration=3000&pause=800&color=00ADD8&center=true&vCenter=true&width=850&lines=CC1352R+TI+BIM%2FOAD+Yerlesimi;Persistent+ve+User+Imaj+Duzeni;0x00030000+Staging+Slotu;OAD+Header+ve+Entry+Analizi;BIL304+HW3+3.+Asama" />

<h1>BIL304 HW3 - CC1352R TI BIM/OAD Firmware Yerlesimi</h1>

<p>
  <img src="https://img.shields.io/badge/Hedef-CC1352R-00ADD8" />
  <img src="https://img.shields.io/badge/OS-Contiki--NG-success" />
  <img src="https://img.shields.io/badge/Boot-TI%20BIM%2FOAD-orange" />
  <img src="https://img.shields.io/badge/Kontrol-ELF%20%2B%20Section-8A2BE2" />
  <img src="https://img.shields.io/badge/Kapsam-3.%20Asama-blue" />
</p>

<p>
  <b>CC1352R uzerinde persistent fallback imaj, user staging imaj,
  TI BIM ve CCFG yerlesimini aciklayan donanim uyarlama calismasi.</b>
</p>

<p>
  Persistent image | User staging image | TI BIM secimi |
  OAD header | CCFG guvenligi
</p>

<p>
  <a href="#projedeki-dosyalar">Dosyalar</a> |
  <a href="#flash-yerlesimi">Flash Yerlesimi</a> |
  <a href="#oad-header-ve-entry">OAD Header</a> |
  <a href="#boot-akisi">Boot Akisi</a> |
  <a href="#dockercontiki-derleme">Derleme</a> |
  <a href="#kontrol-komutlari">Kontrol</a>
</p>

</div>

Bu repo CC1352R uzerinde **TI BIM/OAD** akisiyla iki uygulama imaji
yerlestirmek icin hazirlandi:

- `old-firmware`: persistent/fallback imaj
- `new-firmware`: user/staging imaji
- `bim_onchip`: reset sonrasi gecerli imaji secen TI Boot Image Manager

BIM, flash'taki TI OAD image header'larini okur ve gecerli imajin
`prgEntry` adresindeki vektor tablosuna `jumpToPrgEntry()` ile gecer.

## Projedeki Dosyalar

| Dosya | Gorev |
| --- | --- |
| `old-firmware.c` | Persistent/fallback uygulama. Calisinca yesil LED'i yakar ve OAD yerlesimini loglar. |
| `new-firmware.c` | User/staging uygulama. Calisinca kirmizi LED'i 2 saniyede bir toggle eder. |
| `oad_layout.h` | Flash slotlari, OAD header boyutu, entry adresleri ve firmware versiyonlari. |
| `oad_image_header.h` | Docker/Contiki build icin gereken TI OAD header sabitleri ve struct tanimlari. |
| `oad_hdr_old.c` | Persistent imaj icin TI OAD image header. |
| `oad_hdr.c` | User imaj icin TI OAD image header. |
| `old-firmware.ld` | Persistent imaji `0x00000000` slotuna linkler. |
| `new-firmware.ld` | User imaji `0x00030000` slotuna linkler. |
| `Makefile` | Contiki projelerini ve OAD header kaynaklarini build'e ekler. |
| `project-conf.h` | Contiki log seviyesini ayarlar. |

## Flash Yerlesimi

CC1352R program flash alani `352 KiB` (`0x00058000`) kabul edilir. Sayfa
boyutu `8 KiB` (`0x2000`).

| Alan | Baslangic | Bitis | Boyut | Gorev |
| --- | ---: | ---: | ---: | --- |
| Persistent image | `0x00000000` | `0x0002FFFF` | 192 KiB | Fallback/persistent uygulama |
| User image slot | `0x00030000` | `0x00051FFF` | 136 KiB | Yeni OAD user firmware |
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

Persistent/fallback imaj:

```text
0x00000000  TI OAD image header
0x00000100  reset vectors / prgEntry
```

User/staging imaj:

```text
0x00030000  TI OAD image header
0x00030100  reset vectors / prgEntry
```

`oad_layout.h` icindeki ilgili sabitler:

```c
#define OAD_ACTIVE_IMAGE_BASE       0x00000000UL
#define OAD_STAGING_IMAGE_BASE      0x00030000UL
#define OAD_IMAGE_HEADER_SIZE       0x00000100UL
#define OAD_ACTIVE_ENTRY            (OAD_ACTIVE_IMAGE_BASE + OAD_IMAGE_HEADER_SIZE)
#define OAD_STAGING_ENTRY           (OAD_STAGING_IMAGE_BASE + OAD_IMAGE_HEADER_SIZE)
```

## OAD Header Icerigi

Persistent imaj header'i:

```text
imgType   = OAD_IMG_TYPE_PERSISTENT_APP
startAddr = 0x00000000
prgEntry  = 0x00000100
softVer   = 0x00010000
```

User imaj header'i:

```text
imgType   = OAD_IMG_TYPE_APPSTACKLIB
startAddr = 0x00030000
prgEntry  = 0x00030100
softVer   = 0x00020000
```

Header yapilari repo icindeki TI OAD uyumlu dosyadan gelir:

```c
#include "oad_image_header.h"
```

## Boot Akisi

```text
Reset
  -> ROM/CCFG ayarlari BIM bolgesine gider
  -> BIM APPSTACKLIB user imaj header'ini arar
  -> Gecerli user imaj varsa 0x00030100 adresine atlar
  -> User imaj yoksa/gecersizse persistent imaji arar
  -> Persistent imaj gecerse 0x00000100 adresine atlar
```

Beklenen LED davranisi:

- user imaj calisiyorsa kirmizi LED heartbeat,
- persistent imaj calisiyorsa yesil LED heartbeat.

## BIM Rebuild Ayarlari

Bu layout icin `bim_onchip` projesi yeniden derlenirken user ve persistent
header adresleri acik verilmelidir. Aksi halde stock BIM user imaji varsayilan
page 0'da arar ve `0x00030000` slotundaki imaja gecmeyebilir.

Gerekli preprocessor define'lari:

```text
APP_HDR_LOC
APP_HDR_ADDR=0x00030000
PERSIST_HDR_LOC
PERSIST_HDR_ADDR=0x00000000
```

Onerilen deneme konfigurasyonu `Debug_unsecure`'dir. Release/secure akista
CRC/signature alanlari icin TI OAD Image Tool ciktisi kullanilmalidir.

## Docker/Contiki Derleme

Contiki ortamini Docker icinde kullanirken iki imaj ayri derlenmelidir. Bunun
sebebi Contiki-NG `LDSCRIPT` degiskenini proje bazli degil global kullanmasidir.

Persistent/fallback imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
```

User/staging imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
```

Docker icinde `CONTIKI` yolu otomatik degilse:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 CONTIKI=/path/to/contiki-ng LDSCRIPT=old-firmware.ld old-firmware
make TARGET=simplelink BOARD=sensortag/cc1352r1 CONTIKI=/path/to/contiki-ng LDSCRIPT=new-firmware.ld new-firmware
```

Beklenen ELF dosyalari:

```text
build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
```

## OAD Image Tool

`oad_hdr.c` ve `oad_hdr_old.c` icindeki `crc32`, `len`, `imgEndAddr` ve segment
length alanlari build oncesinde placeholder durumdadir. Release veya secure
akis icin TI OAD Image Tool bu alanlari doldurmalidir.

Aracin parametreleri SDK surumune gore degisebildigi icin build ortaminda once
yardim ciktisi kontrol edilmelidir:

```sh
oad_image_tool --help
```

Windows SDK kurulumunda arac genelde buradadir:

```text
C:\ti\simplelink_cc13xx_cc26xx_sdk_8_32_00_07\tools\common\oad\oad_image_tool.exe
```

## Cihaza Yukleme Sirasi

Bolge bazli programlama yapilmalidir. Full chip erase, son sektordeki BIM/CCFG
alanini silebilir.

1. BIM imaji:

```text
0x00056000 - 0x00057FFF
```

2. Persistent/fallback imaj:

```text
0x00000000 - 0x0002FFFF
```

3. User/staging imaj:

```text
0x00030000 - 0x00051FFF
```

## Kontrol Komutlari

ELF icinde header ve entry adreslerini kontrol etmek icin:

```sh
arm-none-eabi-nm -n build/simplelink/sensortag/cc1352r1/old-firmware.simplelink | grep "_imgHdr\|ResetISR"
arm-none-eabi-nm -n build/simplelink/sensortag/cc1352r1/new-firmware.simplelink | grep "_imgHdr\|ResetISR"
```

Beklenen:

```text
old-firmware: _imgHdr 0x00000000, ResetISR/reset vectors 0x00000100 civari
new-firmware: _imgHdr 0x00030000, ResetISR/reset vectors 0x00030100 civari
```

Section kontrolu:

```sh
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
```

Kontrol edilmesi gerekenler:

- `.image_header` persistent imajda `0x00000000`,
- `.resetVecs` persistent imajda `0x00000100`,
- `.image_header` user imajda `0x00030000`,
- `.resetVecs` user imajda `0x00030100`,
- uygulama imajlari `0x00056000 - 0x00057FFF` BIM/CCFG alanina tasmaz.

## Notlar

- Bu repo TI BIM kaynak projesini icermez; BIM SDK/CCS projesinde yukaridaki
  define'larla rebuild edilmelidir.
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
