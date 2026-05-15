# BIM On-Chip Notes

Bu projede kullanılan bootloader ayrı bir proje olarak `../bim_onchip/`
klasöründe tutulur. `rehydrator` tarafında BIM yeniden yazılmaz; bu klasördeki
hazır TI BIM çıktısının beklediği flash yerleşimine uygun firmware imajları
üretilir.

## BIM Projesine Erişim

BIM'e erişmek için iki pratik yol vardır:

```powershell
cd ..\bim_onchip
```

veya Code Composer Studio içinde:

```text
File -> Import -> CCS Projects -> Select search-directory -> ../bim_onchip
Build configuration -> Debug_unsecure
Target configuration -> targetConfigs/CC1352R1F3.ccxml
```

## Debug Unsecure Seçimi

`Debug_unsecure` yapılandırması bu ödev için yeterlidir. Bu yapılandırmada secure
imza doğrulaması yapılmaz; debug amaçlı LED geri bildirimi vardır ve post-build
CRC/signature adımı zorunlu değildir. Bu yüzden `rehydrator` içindeki OAD header
alanlarında `crc32`, `len`, `imgEndAddr` ve segment uzunluklarının debug senaryoda
`0xFFFFFFFF` kalması beklenen bir durumdur.

## Referans BIM Dosyaları

| Dosya | Bizim için anlamı |
| --- | --- |
| `../bim_onchip/Debug_unsecure/bim_onchip.hex` | Cihaza yazılacak asıl BIM dosyasıdır. Intel HEX formatında olduğu için adresleri dosyanın içinde taşır. UniFlash veya CCS ile doğrudan programlanabilir. |
| `../bim_onchip/Debug_unsecure/bim_onchip.map` | BIM'in flash/RAM yerleşimini doğruladığımız ana rapordur. `FLASH_BIM`, `FLASH_CCFG`, entry point, `.resetVecs`, `.text` ve `.ccfg` adresleri buradan okunur. |
| `../bim_onchip/Debug_unsecure/bim_onchip_linkInfo.xml` | Linker bilgisinin XML karşılığıdır. Otomatik kontrol veya ayrıntılı section analizi gerektiğinde kullanılır. README'deki adreslerin `map` ile aynı olduğunu buradan da doğrulayabiliriz. |
| `../bim_onchip/Debug_unsecure/bim_onchip.out` | Debug/ELF çıktısıdır. CCS ile debug yapılacaksa veya sembol tablosu incelenecekse işe yarar; cihaza normal yükleme için `.hex` tercih edilir. |
| `../bim_onchip/bim_cc26x2_cc13x2.cmd` | BIM'in linker scriptidir. BIM'in neden son flash page'e, CCFG'nin neden en sona yerleştiğini gösterir. `rehydrator/oad_layout.h`, `old-firmware.ld` ve `new-firmware.ld` bu yerleşime göre ayarlanmıştır. |
| `../bim_onchip/README.md` | TI BIM projesinin kullanım notlarıdır. On-chip OAD akışı, `Debug_unsecure` / `Release_unsecure` farkı ve persistent/user image mantığı burada özetlenir. |
| `../bim_onchip/.project`, `.cproject`, `.ccsproject` | CCS'in projeyi tanıması için gereken proje metadata dosyalarıdır. BIM'i CCS'e import ederken bu dosyalar klasörde kalmalıdır. |
| `../bim_onchip/targetConfigs/CC1352R1F3.ccxml` | CCS debug/programlama oturumu için hedef cihaz dosyasıdır. Kart CCS üzerinden bağlanacaksa kullanılır. |
| `../bim_onchip/Debug_unsecure.zip` | `Debug_unsecure` çıktılarının paketlenmiş halidir. Projeyi başka makineye taşırken kullanışlıdır; günlük doğrulamada açılmış klasördeki dosyalar yeterlidir. |

## Gereksiz Build Çıktıları

Derleme sırasında oluşan `Application/*.o`, `Application/*.d`, `SPI/*.o`,
`subdir_rules.mk`, `subdir_vars.mk`, `.clangd` cache dosyaları ve generated
makefile dosyaları bizim firmware yerleşimi kararlarımız için gerekli değildir.
Bunlar CCS build ara çıktılarıdır; asıl referanslarımız `.hex`, `.map`,
`linkInfo.xml` ve linker script dosyasıdır.

## BIM Bellek Yerleşimi

`bim_onchip.map` dosyasından doğrulanan BIM bellek yerleşimi:

| Alan | Başlangıç | Uzunluk | Kullanılan | Not |
| --- | ---: | ---: | ---: | --- |
| `FLASH_BIM` | `0x00056000` | `0x00001F54` | `0x0000057C` | BIM kodu, reset vector ve `OAD_IMG_ID` burada. |
| `FLASH_CERT` | `0x00057F54` | `0x0000004C` | `0x00000000` | Secure akış için sertifika alanı; debug unsecure build'de boş. |
| `FLASH_FNPTR` | `0x00057FA0` | `0x00000004` | `0x00000000` | Function pointer alanı; bu build'de boş. |
| `FLASH_CCFG` | `0x00057FA8` | `0x00000058` | `0x00000058` | Cihaz konfigürasyon alanı. Yanlışlıkla silinmemeli. |
| `SRAM` | `0x20000300` | `0x00013D00` | `0x00000100` | BIM stack'i `0x20013F00 - 0x20013FFF` aralığında. İlk `0x300` byte ROM için ayrılmıştır. |

## Önemli BIM Sembolleri

| Bilgi | Adres / değer |
| --- | ---: |
| BIM entry point | `ResetISR = 0x0005653D` |
| Vector table | `g_pfnVectors = 0x00056000` |
| `.resetVecs` | `0x00056000 - 0x000560D7` |
| `.text` | `0x000560D8 - 0x0005656B` |
| `OAD_IMG_ID` | `0x0005656C` |
| `.ccfg` / `__ccfg` | `0x00057FA8 - 0x00057FFF` |
| `jumpToPrgEntry` | `0x00056547` |
| `imgIDCheck` | `0x000564C9` |
| `readFlashPg` | `0x000564A1` |

## Firmware İçin Çıkan Kararlar

Bu adresler `rehydrator` için şu kararları doğrudan belirler:

1. `0x00056000 - 0x00057FFF` aralığı BIM/CCFG alanıdır; uygulama firmware'leri
   bu alana taşmamalıdır.
2. `new-firmware` page 0 user image olarak `0x00000000` adresinden başlar ve
   entry adresi `0x00000100` olur.
3. `old-firmware` persistent fallback image olarak `0x00030000` adresinden başlar
   ve entry adresi `0x00030100` olur.
4. Firmware linker scriptleri `.ccfg` section'ını discard eder; CCFG sadece BIM
   HEX dosyasından gelmelidir.
5. Programlama sırasında `.hex` dosyaları tercih edilir; çünkü BIM ve firmware
   adresleri HEX kayıtlarının içinde zaten vardır.

## BIM Hızlı Kontrol

BIM dosyasını hızlı kontrol etmek için:

```powershell
cd ..\bim_onchip
Select-String -Path Debug_unsecure\bim_onchip.map -Pattern "ENTRY POINT|FLASH_BIM|FLASH_CCFG|\.resetVecs|\.ccfg"
Get-Content -Encoding ASCII Debug_unsecure\bim_onchip.hex | Select-Object -First 5
Get-Content -Encoding ASCII Debug_unsecure\bim_onchip.hex | Select-Object -Last 5
```

`bim_onchip.hex` içinde ilk veri kayıtları `0x00056000` çevresine, son kayıtlar
ise `0x00057FA8` CCFG alanına gider. Yani bu dosya hem BIM kodunu hem CCFG
bilgisini taşır. Bu yüzden BIM'i yükledikten sonra full chip erase yapılırsa boot
zinciri bozulur; firmware güncellerken sadece ilgili firmware slotları
programlanmalıdır.
