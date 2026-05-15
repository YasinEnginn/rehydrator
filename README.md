<div align="center">

<img src="https://readme-typing-svg.demolab.com?font=Fira+Code&size=24&duration=3000&pause=800&color=00ADD8&center=true&vCenter=true&width=850&lines=BIL304+HW3+3.+A%C5%9Fama;CC1352R+Ger%C3%A7ekleme;Donan%C4%B1m+Uyarlama;TI+BIM%2FOAD+Yerle%C5%9Fimi" />

<h1>BIL304 HW3 - 3. Aşama</h1>

<p>
  <img src="https://img.shields.io/badge/Hedef-CC1352R-00ADD8" />
  <img src="https://img.shields.io/badge/OS-Contiki--NG-success" />
  <img src="https://img.shields.io/badge/Boot-TI%20BIM%2FOAD-orange" />
  <img src="https://img.shields.io/badge/Kapsam-Donan%C4%B1m%20Uyarlama-blue" />
</p>

<p>
  <b>CC1352R Gerçekleme ve Donanım Uyarlama Görevi</b>
</p>

<p>
  Mevcut BIM korunur | Old firmware fallback 0x30000 | New firmware page 0
</p>

</div>

**Rehydrator** *(ri-HAY-drey-tır)* adı, reset veya yeniden başlatma sonrasında
bir düğümün kalıcı state/rol konfigürasyonunu flash'tan okuyup sistemi yeniden
ayağa kaldırması fikrinden gelir. Bu projede de firmware imajları, mevcut TI BIM
akışı ve flash yerleşimi korunarak cihazın doğru imajdan güvenilir biçimde
başlaması hedeflenir.

Bu depo, ödevin yalnızca **3. CC1352R Gerçekleme ve Donanım Uyarlama** kısmı
için hazırlanmıştır. Amaç, Contiki-NG ile derlenen iki firmware imajını CC1352R
flash yerleşimine uyarlamak ve mevcut TI BIM/OAD akışıyla çalışabilecek şekilde
doğrulamaktır.

## Quick Start

Tek komutla eski fallback imajı ve yeni user/update imajı için yükleme dosyaları
üretilebilir:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 upload-files
```

Üretilen dosyalar:

```text
upload/old-firmware.hex
upload/new-firmware.hex
upload/old-firmware.bin
upload/new-firmware.bin
```

UniFlash ile yükleme yaparken mümkünse `.hex` dosyalarını kullan. HEX formatında
flash adresleri dosyanın içinde gömülü olduğu için `old-firmware` ve
`new-firmware` doğru slotlara yerleşir.

Temiz build gerektiğinde:

```sh
rm -rf build upload
make TARGET=simplelink BOARD=sensortag/cc1352r1 upload-files
```

Build sonrasında temel section adreslerini kontrol etmek için:

```sh
scripts/verify-layout.sh
```

## Doküman Haritası

README kısa tutulmuştur; ayrıntılı teknik açıklamalar `docs/` altındadır.

| Dosya | İçerik |
| --- | --- |
| [docs/flash-layout.md](docs/flash-layout.md) | Flash/RAM yerleşimi, boot akışı, CCFG notu, ikinci firmware stratejisi ve ödev soruları |
| [docs/build-and-verification.md](docs/build-and-verification.md) | Derleme komutları, upload artifact üretimi, Docker'dan dosya alma, section/header doğrulaması |
| [docs/bim-onchip-notes.md](docs/bim-onchip-notes.md) | TI BIM projesi, `Debug_unsecure` seçimi, BIM map/symbol adresleri ve çıkan kararlar |
| [docs/uniflash-guide.md](docs/uniflash-guide.md) | UniFlash ile güvenli yükleme sırası, HEX/BIN farkı ve beklenen LED davranışı |
| [docs/references.md](docs/references.md) | TI, UniFlash ve Contiki-NG kaynak bağlantıları |
| [scripts/verify-layout.sh](scripts/verify-layout.sh) | Build sonrası `.image_header` ve `.resetVecs` adreslerini kontrol eden yardımcı script |

## Kapsam

Yapılanlar:

- `old-firmware`, eski ve ilk yüklenecek persistent fallback imaj olarak
  `0x00030000` slotuna yerleştirildi.
- `new-firmware`, yeni firmware/user imaj olarak page 0'a yerleştirildi.
- Her iki imaj için TI OAD uyumlu image header eklendi.
- Linker scriptlerde OAD header, reset vector, DMA, stack ve RAM bölgeleri
  düzenlendi.
- ELF section ve OAD header içerikleri `readelf` / `objdump` ile doğrulanabilir
  hale getirildi.

Kapsam dışı:

- BIM kaynak kodunu değiştirmek.
- Yeni bootloader yazmak.
- Release/secure OAD imzalama ve CRC paketleme akışını otomatikleştirmek.

## Mimari Özet

Bu çalışmada BIM dosyası değiştirilmez. Firmware tarafındaki linker script, OAD
header ve flash adresleri mevcut BIM'in arama sırasına göre düzenlenir.

| İmaj | Flash slotu | Header | Entry / Reset Vector | Image type |
| --- | ---: | ---: | ---: | --- |
| `new-firmware` | `0x00000000 - 0x0002FFFF` | `0x00000000` | `0x00000100` | `APPSTACKLIB` |
| `old-firmware` | `0x00030000 - 0x00051FFF` | `0x00030000` | `0x00030100` | `PERSISTENT_APP` |
| BIM + CCFG | `0x00056000 - 0x00057FFF` | BIM tarafında | `ResetISR = 0x0005653D` | TI BIM |

Mevcut BIM önce page 0'daki user image'ı kontrol eder. Geçerli `new-firmware`
yoksa persistent fallback slotundaki `old-firmware` imajına geçer.

## Kod Dosyaları

| Dosya | Görev |
| --- | --- |
| `old-firmware.c` | Persistent fallback uygulaması; slot bilgisini loglar ve yeşil LED heartbeat üretir. |
| `new-firmware.c` | User/update firmware uygulaması; sürüm/slot bilgisini loglar ve kırmızı LED heartbeat üretir. |
| `oad_hdr.c` | User imaj için `_imgHdr` OAD image header nesnesini tanımlar. |
| `oad_hdr_old.c` | Persistent fallback imaj için `_imgHdr` OAD image header nesnesini tanımlar. |
| `oad_image_header.h` | TI OAD header alanları için sabitleri ve packed struct tanımlarını içerir. |
| `oad_layout.h` | Flash slot adresleri, entry adresleri ve firmware versiyonlarını merkezi olarak tanımlar. |
| `new-firmware.ld` | User imaj linker scriptidir; header'ı `0x00000000`, reset vector'ı `0x00000100` adresine yerleştirir. |
| `old-firmware.ld` | Persistent fallback linker scriptidir; header'ı `0x00030000`, reset vector'ı `0x00030100` adresine yerleştirir. |
| `Makefile` | Hedef seçimini, OAD header kaynağını ve `.hex` / `.bin` üretimini yönetir. |
| `project-conf.h` | Proje seviyesinde Contiki ayarlarını içerir. |

## Repo Hijyeni

- Generated firmware çıktıları repo içinde tutulmaz; `upload/`, `build/`,
  `*.hex`, `*.bin`, `*.elf`, `*.out`, `*.map`, `*.o`, `*.d` ignore edilir.
- Teslim/release için `.hex` ve `.bin` dosyaları GitHub Release eki olarak,
  yanlarına `sha256sums.txt` eklenerek paylaşılmalıdır.
- Issue template, security notu, changelog ve GitHub Actions kontrolleri repo
  içinde bulunur.

## Beklenen Çıktılar

| Beklenen çıktı | Nerede? |
| --- | --- |
| Flash ve RAM kullanımı | [docs/flash-layout.md](docs/flash-layout.md) |
| Boot zinciri | [docs/flash-layout.md](docs/flash-layout.md) |
| CCFG alanının neden kritik olduğu | [docs/flash-layout.md](docs/flash-layout.md) ve [docs/uniflash-guide.md](docs/uniflash-guide.md) |
| İkinci firmware yerleşim stratejisi | [docs/flash-layout.md](docs/flash-layout.md) |
| ELF/section doğrulaması | [docs/build-and-verification.md](docs/build-and-verification.md) |
| Cihaza yüklenecek dosyaların üretimi | [docs/build-and-verification.md](docs/build-and-verification.md) |
| UniFlash yükleme sırası | [docs/uniflash-guide.md](docs/uniflash-guide.md) |

## Not

Debug BIM denemesinde OAD header içindeki `crc32`, `len`, `imgEndAddr` ve segment
length alanlarının `0xFFFFFFFF` kalması kabul edilebilir. Release veya secure OAD
akışı için bu alanlar TI OAD Image Tool ile doldurulmalıdır.
