# Glossary

Bu sözlük, projede ve ödev raporunda geçen terimleri kısa ama bağlamlı biçimde
açıklar. Terimler alfabetik değil, konu gruplarına göre düzenlenmiştir.

## Proje ve Platform

| Terim | Açıklama |
| --- | --- |
| Rehydrator | Reset veya yeniden başlatma sonrası düğümün kalıcı state/rol konfigürasyonunu flash'tan okuyup sistemi yeniden ayağa kaldırması fikrini temsil eden proje adı. |
| CC1352R | TI SimpleLink ailesinden, bu ödevde hedeflenen mikrodenetleyici. |
| CC1352R1F3 | UniFlash/CCS tarafında seçilen hedef cihaz varyantı. |
| SensorTag / `sensortag/cc1352r1` | Contiki-NG build komutunda kullanılan board seçimi. |
| Contiki-NG | Bu projede firmware imajlarının derlendiği gömülü işletim sistemi. |
| SimpleLink | TI'nin CC13xx/CC26xx platform ailesi ve Contiki-NG `TARGET` değeri. |
| `TARGET=simplelink` | Contiki-NG derlemesinde SimpleLink platformunu seçer. |
| `BOARD=sensortag/cc1352r1` | CC1352R SensorTag board yapılandırmasını seçer. |

## Boot, BIM ve OAD

| Terim | Açıklama |
| --- | --- |
| Boot chain | Reset sonrası ROM boot, CCFG ayarları, BIM ve seçilen uygulama imajı arasında ilerleyen açılış zinciri. |
| ROM boot | Çip içinde sabit bulunan ilk boot kodu. |
| BIM | Boot Image Manager. TI'nin OAD akışında geçerli firmware imajını bulan ve o imaja atlayan boot yöneticisi. |
| OAD | Over-the-Air Download. TI ekosisteminde firmware update ve image seçimi için kullanılan model. Bu projede update paketleme otomasyonu değil, OAD header ve BIM uyumlu yerleşim uygulanır. |
| On-chip OAD | Birden fazla firmware imajının aynı cihaz flash'ı içinde tutulduğu OAD yaklaşımı. |
| Bootloader | Reset sonrası uygulama firmware'ine geçmeden önce çalışan kod. Bu projede yeni bootloader yazılmaz; mevcut TI BIM kullanılır. |
| CCFG | Customer Configuration. Cihazın boot davranışını etkileyen kritik flash konfigürasyon alanı. BIM HEX dosyasından gelmelidir. |
| `Debug_unsecure` | Secure imza doğrulaması gerektirmeyen debug BIM build yapılandırması. Bu ödev için yeterli kabul edilir. |
| Secure / Release OAD | CRC, imza veya image tool adımlarıyla daha sıkı doğrulama yapan üretim akışı. Bu proje kapsamı dışındadır. |

## Firmware İmajları ve Slotlar

| Terim | Açıklama |
| --- | --- |
| Firmware image | Cihazda çalışacak uygulama imajı. Bu projede `old-firmware` ve `new-firmware` olarak iki imaj vardır. |
| `old-firmware` | İlk yüklenecek eski/persistent fallback uygulama imajı. |
| `new-firmware` | Güncelleme sonrası page 0'a yazılacak yeni user/update uygulama imajı. |
| Slot | Flash içinde bir firmware imajı için ayrılan adres aralığı. |
| Page 0 | BIM'in öncelikli baktığı user image alanı. Bu projede `0x00000000 - 0x0002FFFF`. |
| User image | Normalde öncelikli çalıştırılan uygulama imajı. Bu projede `new-firmware`. |
| Persistent fallback | User image yoksa veya geçersizse çalıştırılan kalıcı fallback imajı. Bu projede `old-firmware`. |
| `APPSTACKLIB` | TI OAD header içinde user/application stack image tipini belirten image type. |
| `PERSISTENT_APP` | TI OAD header içinde persistent fallback image tipini belirten image type. |
| Metadata | Update akışı için ayrılmış flash alanı. Bu projede kullanılmaz ama layout içinde ayrılmıştır. |
| Recovery | Kurtarma/recovery akışı için ayrılmış flash alanı. Bu projede kullanılmaz ama layout içinde ayrılmıştır. |
| Staging | Yeni imajın aktif alana kopyalanmadan önce tutulabileceği ara alan. Mevcut BIM modeliyle doğrudan kullanılmaz; ek bootloader/recovery mantığı gerekir. |

## Adresler ve Section'lar

| Terim | Açıklama |
| --- | --- |
| Flash | Kalıcı program belleği. Firmware imajları, OAD header, BIM ve CCFG burada tutulur. |
| SRAM | Çalışma zamanı RAM alanı. `.data`, `.bss`, stack ve DMA gibi runtime verileri burada kullanılır. |
| GPRAM | CC1352R içindeki cache/GPRAM bölgesi. |
| Header | Firmware imajının başındaki OAD metadata alanı. |
| Entry / `prgEntry` | BIM'in doğrulama sonrası atlayacağı uygulama başlangıç adresi. |
| Reset vector | CPU reset sonrası uygulama başlangıç tablosu. Bu projede header'dan sonra `0x100` offsetine yerleşir. |
| `.image_header` | OAD image header nesnesinin yerleştirildiği linker section. |
| `.resetVecs` | Reset vector tablosunun yerleştirildiği section. |
| `.text` | Uygulama kodunun bulunduğu flash section. |
| `.data` | Flash'tan RAM'e kopyalanacak initialized data section. |
| `.bss` | RAM'de sıfırlanacak veri alanı. |
| `.stack` | Stack için ayrılan RAM alanı. |
| `.ccfg` | Cihaz konfigürasyon section'ı. Uygulama imajlarında discard edilir; CCFG BIM HEX dosyasından gelmelidir. |
| DMA | Direct Memory Access. Contiki platformunda bazı çevre birimleri için RAM tabloları kullanabilir. |

## OAD Header Alanları

| Terim | Açıklama |
| --- | --- |
| `_imgHdr` | OAD image header struct nesnesi. `oad_hdr.c` ve `oad_hdr_old.c` içinde tanımlanır. |
| `imgID` | İmaj kimliği. Bu projede `CC13x2R1`. |
| `crc32` | Image CRC alanı. Debug BIM senaryosunda `0xFFFFFFFF` kalması kabul edilir. |
| `bimVer` | BIM sürüm bilgisi. |
| `metaVer` | OAD metadata/header sürüm bilgisi. |
| `techType` | Kablosuz teknoloji türü. Bu projede proprietary RF sabiti kullanılır. |
| `imgCpStat` | Image copy status alanı. |
| `crcStat` | CRC durumu alanı. |
| `imgType` | İmaj tipini belirler: `APPSTACKLIB` veya `PERSISTENT_APP`. |
| `imgNo` | Image number alanı. |
| `imgVld` | Image validity alanı. |
| `len` | Image length alanı. Debug BIM senaryosunda `0xFFFFFFFF` kalabilir. |
| `softVer` | Firmware yazılım versiyonu. `old-firmware` için `00000100`, `new-firmware` için `00000200`. |
| `imgEndAddr` | Image bitiş adresi alanı. Debug BIM senaryosunda `0xFFFFFFFF` kalabilir. |
| `hdrLen` | Header uzunluğu. |
| `rfu` | Reserved for future use. İleride kullanım için ayrılmış alan. |
| `imgPayload` | Header içindeki payload/segment metadata alanı. |
| `segTypeImg` | Image payload segment tipini belirtir. |
| `wirelessTech` | Payload tarafındaki wireless teknoloji bilgisi. |
| `imgSegLen` | Image segment length alanı. Debug BIM senaryosunda `0xFFFFFFFF` kalabilir. |
| `startAddr` | İmajın flash başlangıç adresi. |
| `0xFFFFFFFF` | Debug/unfilled header alanlarında kullanılan default/invalid değer. |
| `0xFF` padding | Header sonu ile reset vector başlangıcı arasındaki boş alanın dolgu değeri. |

## Build, Artifact ve Araçlar

| Terim | Açıklama |
| --- | --- |
| `Makefile` | Hedef seçimi, linker script seçimi ve upload artifact üretimini yönetir. |
| `LDSCRIPT` | Contiki-NG build sistemine verilecek linker script değişkeni. |
| Linker script | Section'ların flash/RAM adreslerini belirleyen `.ld` dosyası. |
| `PROJECT_SOURCEFILES` | Makefile içinde hedefe göre `oad_hdr.c` veya `oad_hdr_old.c` eklemek için kullanılır. |
| ELF | Link sonrası üretilen sembollü firmware çıktısı. `readelf` ve `objdump` ile incelenir. |
| HEX | Intel HEX formatı. Adres bilgisi dosyanın içinde taşındığı için UniFlash için tercih edilir. |
| BIN | Raw binary çıktı. Adres bilgisi içermez; programlama aracında başlangıç adresi elle verilmelidir. |
| MAP | Linker'ın bellek ve symbol yerleşim raporu. |
| OUT | CCS/debug tarafında kullanılan ELF benzeri debug çıktısı. |
| `arm-none-eabi-objcopy` | ELF dosyasından HEX/BIN üreten araç. |
| `arm-none-eabi-readelf` | ELF section adreslerini okumak için kullanılan araç. |
| `arm-none-eabi-objdump` | Section içeriğini dump etmek için kullanılan araç. |
| `upload-files` | Makefile hedefi; iki imajı derleyip `.hex` ve `.bin` üretir. |
| `build/` | Contiki-NG build çıktılarının üretildiği klasör. Git'e alınmaz. |
| `upload/` | Cihaza yazılacak demo artifact'lerin üretildiği klasör. Git'e alınmaz. |
| `sha256sums.txt` | Release artifact'lerinin bütünlüğünü doğrulamak için önerilen checksum dosyası. |

## Programlama ve Güvenlik

| Terim | Açıklama |
| --- | --- |
| UniFlash | TI'nin cihaz programlama aracı. |
| XDS110 | TI kartlarında yaygın kullanılan debug/programlama arayüzü. |
| Mass erase / full chip erase | Tüm flash'ı silen işlem. BIM/CCFG alanını bozabileceği için normal test akışında kullanılmamalıdır. |
| Region-based programming | Yalnızca seçilen HEX dosyasının kapsadığı flash bölgelerini yazma yaklaşımı. |
| Sector erase | Flash'ın belirli sektörlerini silme işlemi. Yanlış sektör seçilirse çalışan imaj veya BIM bozulabilir. |
| Checksum / CRC | Artifact veya image bütünlüğünü kontrol etmek için kullanılan özet/doğrulama değeri. |
| GitHub Release artifact | Repo içinde takip edilmeyen `.hex` / `.bin` çıktılarının yayınlanması için önerilen yer. |
