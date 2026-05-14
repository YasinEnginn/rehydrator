# BIL304 HW3 - CC1352R TI BIM/OAD Modeli

Bu depo artik tek bir yaklasimi hedefler: **TI BIM/OAD modeli**.
Ozel `HW3` container header'i, container paketleme araci ve uygulama icinde
staging CRC dogrulamasi kaldirildi.

Geri donus noktasi olarak mevcut calisma baslamadan once su branch birakildi:

```text
backup/pre-ti-bim-oad
```

Sorun olursa:

```sh
git switch backup/pre-ti-bim-oad
```

## Flash Yerlesimi

| Alan | Baslangic | Bitis | Boyut | Gorev |
| --- | ---: | ---: | ---: | --- |
| Persistent image | `0x00000000` | `0x0002FFFF` | 192 KiB | Fallback/persistent uygulama |
| User image slot | `0x00030000` | `0x00051FFF` | 136 KiB | Yeni OAD user firmware |
| Update metadata | `0x00052000` | `0x00053FFF` | 8 KiB | Ayrilmis alan |
| Recovery/reserved | `0x00054000` | `0x00055FFF` | 8 KiB | Ayrilmis alan |
| BIM + CCFG | `0x00056000` | `0x00057FFF` | 8 KiB | TI BIM ve CCFG |

Her uygulama slotunun ilk `0x100` byte'i TI OAD image header icin ayrilir.
Reset vektorleri hemen ardindan baslar:

```text
Persistent image:
0x00000000  OAD image header
0x00000100  reset vectors / prgEntry

User image:
0x00030000  OAD image header
0x00030100  reset vectors / prgEntry
```

## Kodun Mantigi

`old-firmware.c`, artik eski `HW3` staging container'ini okumaz. TI BIM
tarafindan secildiginde persistent/fallback uygulama olarak calisir, yesil LED'i
yakar ve bellek yerlesimini loglar.

`new-firmware.c`, user OAD imajidir. TI BIM bu imaji sectiginde `0x00030100`
adresindeki entry noktasina gecer; uygulama kirmizi LED'i iki saniyede bir
toggle eder.

`oad_hdr_old.c`, persistent imaj icin TI OAD header uretir:

- `imgType = OAD_IMG_TYPE_PERSISTENT_APP`
- `startAddr = 0x00000000`
- `prgEntry = 0x00000100`

`oad_hdr.c`, yeni user imaji icin TI OAD header uretir:

- `imgType = OAD_IMG_TYPE_APPSTACKLIB`
- `startAddr = 0x00030000`
- `prgEntry = 0x00030100`

## Derleme

Contiki-NG `LDSCRIPT` degiskenini proje bazli degil, global kullanir. Bu yuzden
iki imaj ayri derlenmelidir.

Persistent/fallback imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
```

User/staging imaji:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
```

Bu makinede `CONTIKI` yolu depo icinden otomatik bulunmuyor. Gerekirse komuta
asagidaki gibi acik yol eklenmelidir:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 CONTIKI=C:/path/to/contiki-ng LDSCRIPT=new-firmware.ld new-firmware
```

## OAD Image Tool

TI dokumanina gore OAD header icindeki CRC, length ve image end alanlari
elle doldurulmamalidir. Bunlar TI'nin **OAD Image Tool** post-build adimiyla
uretilir.

Bu makinede arac burada bulunuyor:

```text
C:\ti\simplelink_cc13xx_cc26xx_sdk_8_32_00_07\tools\common\oad\oad_image_tool.exe
```

Release BIM kullaniliyorsa, cihaza yuklenecek uygulama imaji OAD Image Tool
ciktisi olmalidir. Debug_unsecure BIM ile IDE/hex denemesi daha toleranslidir,
ama nihai akista OAD Image Tool kullanilmalidir.

## Cihaza Yukleme Sirasi

Bolge bazli programlama yapilmalidir. Full chip erase yapilirsa son sektordeki
BIM/CCFG silinebilir.

Onemli: Bu flash yerlesimi icin BIM, user imaj header'ini `0x00030000`
adresinde, persistent imaj header'ini `0x00000000` adresinde aramalidir.
`bim_onchip` projesini rebuild ederken Debug_unsecure veya Release_unsecure
konfigurasyonuna su preprocessor define'lari eklenmelidir:

```text
APP_HDR_LOC
APP_HDR_ADDR=0x00030000
PERSIST_HDR_LOC
PERSIST_HDR_ADDR=0x00000000
```

Bu define'lar olmadan SDK'deki stock BIM once yalnizca page 0'da
`APPSTACKLIB` arar; user imaji `0x00030000` slotunda oldugu icin beklenen
sekilde yeni firmware'e gecmeyebilir.

1. BIM imajini yukle:

```text
..\bim_onchip\Debug_unsecure\bim_onchip.hex
```

Hedef aralik:

```text
0x00056000 - 0x00057FFF
```

2. Persistent imaji yukle:

```text
0x00000000 - 0x0002FFFF
```

3. User imajini yukle:

```text
0x00030000 - 0x00051FFF
```

Reset sonrasi BIM once gecerli `APPSTACKLIB` user imaji arar. Gecerli user imaji
bulunursa `0x00030100` adresine gecer. Bulamazsa persistent/fallback imaja
donebilir.

## Kontrol Komutlari

ELF icinde OAD header ve reset entry adreslerini kontrol etmek icin:

```sh
arm-none-eabi-nm -n build/simplelink/sensortag/cc1352r1/old-firmware.simplelink | grep "_imgHdr\|ResetISR"
arm-none-eabi-nm -n build/simplelink/sensortag/cc1352r1/new-firmware.simplelink | grep "_imgHdr\|ResetISR"
```

Beklenen:

```text
old-firmware: _imgHdr 0x00000000, ResetISR/reset vectors 0x00000100 civari
new-firmware: _imgHdr 0x00030000, ResetISR/reset vectors 0x00030100 civari
```

## Kaynak Notlari

TI OAD image header dokumani, BIM'in OAD image header'a dayandigini ve program
entry alanini kullandigini belirtir:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_sdk/1.60.00.29_new/exports/docs/ti154stack/html/oad/image-header.html

TI OAD Image Tool dokumani, hex/bin donusumu, padding ve CRC'nin post-build
araciyla yapilmasi gerektigini aciklar:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_26x2_sdk/4.10.00.78/exports/docs/proprietary-rf/proprietary-rf-users-guide/oad/tools.html

TI OAD linker ornegi, image header icin `0x100` byte ayirmayi ve reset
vektorlerini header'in arkasina yerlestirmeyi gosterir:

https://software-dl.ti.com/simplelink/esd/simplelink_cc13x2_26x2_sdk/4.40.04.04/exports/docs/thread/html/thread-oad/create-tiop-oad-image.html
