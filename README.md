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
  Mevcut BIM korunur | User firmware page 0 | Persistent fallback 0x30000
</p>

</div>

Bu depo, ödevin yalnızca **3. CC1352R Gerçekleme ve Donanım Uyarlama**
kısmı için hazırlanmıştır. Amaç, Contiki-NG ile derlenen iki firmware imajını
CC1352R flash yerleşimine uyarlamak ve mevcut TI BIM/OAD akışıyla
çalışabilecek şekilde doğrulamaktır.

Bu çalışmada BIM dosyası değiştirilmez. Firmware tarafındaki linker script,
OAD header ve flash adresleri mevcut BIM'in arama sırasına göre düzenlenir.

## Kapsam

Yapılanlar:

- `new-firmware`, user imaj olarak page 0'a yerleştirildi.
- `old-firmware`, persistent fallback imaj olarak `0x00030000` slotuna yerleştirildi.
- Her iki imaj için TI OAD uyumlu image header eklendi.
- Linker scriptlerde OAD header, reset vector, DMA, stack ve RAM bölgeleri düzenlendi.
- ELF section ve OAD header içerikleri `readelf` / `objdump` ile doğrulandı.

Kapsam dışı:

- BIM kaynak kodunu değiştirmek.
- Yeni bootloader yazmak.
- Release/secure OAD imzalama ve CRC paketleme akışını otomatikleştirmek.

## Bellek Yerleşimi

CC1352R için temel bellek alanları:

| Bellek | Adres Aralığı | Kullanım |
| --- | ---: | --- |
| Flash | `0x00000000 - 0x00057FFF` | Uygulama imajları, OAD header, BIM ve CCFG |
| SRAM | `0x20000000 - 0x20013FFF` | Çalışma zamanı veri, stack, bss ve DMA tabloları |
| GPRAM | `0x11000000 - 0x11001FFF` | Cache/GPRAM bölgesi |
| ROM | Cihaz içinde sabit | TI ROM boot kodu ve yardımcı rutinler |

Flash içindeki firmware yerleşimi:

| Alan | Başlangıç | Bitiş | Görev |
| --- | ---: | ---: | --- |
| User image | `0x00000000` | `0x0002FFFF` | BIM'in ilk aradığı `APPSTACKLIB` imaj |
| Persistent fallback | `0x00030000` | `0x00051FFF` | User imaj yoksa/geçersizse fallback |
| Metadata | `0x00052000` | `0x00053FFF` | Ayrılmış alan |
| Recovery | `0x00054000` | `0x00055FFF` | Ayrılmış alan |
| BIM + CCFG | `0x00056000` | `0x00057FFF` | Mevcut TI BIM ve CCFG |

Her imaj slotunun ilk `0x100` byte'ı OAD image header için ayrılır.
Reset vector tablosu header alanından sonra başlar.

| İmaj | Header | Entry / Reset Vector | Image Type |
| --- | ---: | ---: | --- |
| `new-firmware` | `0x00000000` | `0x00000100` | `APPSTACKLIB` |
| `old-firmware` | `0x00030000` | `0x00030100` | `PERSISTENT_APP` |

## Boot Akışı

Mevcut BIM şu sırayla imaj arar:

```text
Reset
  -> ROM/CCFG ayarları BIM bölgesine yönlendirir
  -> BIM page 0'da APPSTACKLIB user imajı arar
  -> Geçerli user imaj varsa 0x00000100 adresine atlar
  -> User imaj yoksa/geçersizse PERSISTENT_APP fallback imajı arar
  -> Fallback imaj geçerse 0x00030100 adresine atlar
```

Beklenen davranış:

- `new-firmware` çalışıyorsa kırmızı LED heartbeat verir.
- `old-firmware` çalışıyorsa yeşil LED heartbeat verir.

## Sorular ve Cevaplar

Bu bölüm, ödevde istenen soruların güncel TI/Contiki kaynaklarına göre kısa
cevaplarıdır.

| Soru | Cevap |
| --- | --- |
| Uygulamanın çalışan ana imajı nereye yerleşecek? | Mevcut BIM önce page 0'da user image aradığı için `new-firmware` `0x00000000` adresine yerleşir. Entry adresi `0x00000100` olur. |
| Diske/flash'a kaydedilecek ikinci imaj hangi alana yazılacak? | Bu çalışmada ikinci imaj için internal flash'ta `0x00030000 - 0x00051FFF` aralığı ayrıldı. Burada `old-firmware` persistent fallback imajı tutulur. |
| Aynı anda iki tam imaj saklanabiliyor mu? | Bu örnek imajlar küçük olduğu için evet. TI on-chip OAD dokümanı ise bunun flash yerleşimine ve user uygulamanın yeterince küçük olmasına bağlı olduğunu belirtir. |
| Sadece staging alanı varsa aktivasyon nasıl yapılır? | Staging imajı doğrudan seçilmeyecekse BIM/bootloader/recovery kodu tarafından doğrulanıp aktif alana kopyalanmalı veya OAD header üzerinden seçilebilir hale getirilmelidir. Bu projede mevcut BIM'e uyumlu iki seçilebilir slot kullanıldı. |
| Flash erase/write işlemleri mevcut çalışan imajı nasıl etkiler? | Flash silme işlemi sektör bazlıdır. Çalışan imajın veya BIM/CCFG alanının bulunduğu sektör silinirse cihaz boot edemeyebilir. Bu yüzden full chip erase yerine bölge bazlı programlama kullanılır. |

## Beklenen Çıktılar

Ödevin 3. kısmı için beklenen teknik çıktılar bu repoda şu şekilde karşılanır:

| Beklenen çıktı | Repodaki karşılığı |
| --- | --- |
| Flash ve RAM kullanımını gösteren bellek tablosu | `Bellek Yerleşimi` bölümü ve `oad_layout.h` |
| Boot zincirini gösteren kısa akış diyagramı | `Boot Akışı` bölümü |
| CCFG alanının neden kritik olduğu | `CCFG Notu` bölümü |
| İkinci firmware kaydı için yerleşim stratejisi | `İkinci Firmware Stratejisi` bölümü |
| Tam firmware değiştirme için neden bootloader gerektiği | Staging/aktivasyon açıklaması ve mevcut BIM/OAD seçimi |
| ELF/section doğrulaması | `readelf` ve `objdump` komutlarıyla `.image_header`, `.resetVecs`, OAD header alanları |

## CCFG Notu

CCFG, reset sonrası cihaz davranışını ve boot zincirini etkileyen kritik flash
alanıdır. Bu projede BIM + CCFG bölgesi son sektörde tutulur:

```text
0x00056000 - 0x00057FFF
```

Bu bölge yanlışlıkla silinirse cihaz beklenen BIM akışına giremeyebilir.
Bu yüzden yükleme yaparken full chip erase yerine bölge bazlı programlama
kullanılmalıdır.

## İkinci Firmware Stratejisi

Mevcut BIM değiştirilmediği için ikinci imaj için ayrı bir flash slotu
kullanılır. Bu çalışmada `0x00030000 - 0x00051FFF` aralığı persistent fallback
imaj için ayrılmıştır. Böylece page 0'daki user imaj çalışmazsa BIM bu
fallback imaja geçebilir.

Tam firmware değiştirme veya staging alanındaki imajı aktif alana kopyalama
gibi işlemler için ek bootloader/recovery mantığı gerekir. Bu çalışmada yeni
bootloader yazılmamış, mevcut TI BIM/OAD modeli kullanılmıştır.

## Proje Dosyaları

| Dosya | Görev |
| --- | --- |
| `new-firmware.c` | User firmware uygulaması |
| `old-firmware.c` | Persistent fallback uygulaması |
| `oad_hdr.c` | User imaj için OAD header |
| `oad_hdr_old.c` | Persistent fallback imaj için OAD header |
| `oad_image_header.h` | Gerekli TI OAD header sabitleri ve struct tanımları |
| `oad_layout.h` | Flash adresleri ve firmware versiyonları |
| `new-firmware.ld` | User imajı page 0'a linkler |
| `old-firmware.ld` | Persistent fallback imajı `0x00030000` slotuna linkler |
| `Makefile` | Contiki build kuralları |

## Derleme

Contiki-NG `LDSCRIPT` değişkenini global kullandığı için iki imaj ayrı
derlenmelidir.

User imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
```

Persistent fallback imaj:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
```

## Doğrulama

Section adreslerini kontrol etmek için:

```sh
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
arm-none-eabi-readelf -S build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
```

Beklenen:

```text
new-firmware: .image_header 0x00000000, .resetVecs 0x00000100
old-firmware: .image_header 0x00030000, .resetVecs 0x00030100
```

OAD header içeriğini kontrol etmek için:

```sh
arm-none-eabi-objdump -s -j .image_header build/simplelink/sensortag/cc1352r1/new-firmware.simplelink
arm-none-eabi-objdump -s -j .image_header build/simplelink/sensortag/cc1352r1/old-firmware.simplelink
```

Kontrol edilen temel alanlar:

```text
new-firmware:
  magic     = CC13x2R1
  imgType   = 0x07
  prgEntry  = 0x00000100
  startAddr = 0x00000000

old-firmware:
  magic     = CC13x2R1
  imgType   = 0x00
  prgEntry  = 0x00030100
  startAddr = 0x00030000
```

## Cihaza Yükleme

Full chip erase yapılmamalıdır; BIM/CCFG bölgesi korunmalıdır.

Yükleme sırası:

```text
1. BIM             -> 0x00056000 - 0x00057FFF
2. new-firmware    -> 0x00000000 - 0x0002FFFF
3. old-firmware    -> 0x00030000 - 0x00051FFF
```

## Not

Debug BIM denemesinde OAD header içindeki `crc32`, `len`, `imgEndAddr` ve
segment length alanlarının `0xFFFFFFFF` kalması kabul edilebilir. Release veya
secure OAD akışı için bu alanlar TI OAD Image Tool ile doldurulmalıdır.

## Kaynaklar

- TI CC1352R ürün sayfası: https://www.ti.com/product/CC1352R
- TI CC1352R datasheet: https://www.ti.com/lit/ds/symlink/cc1352r.pdf
- TI güncel BIM dokümanı: https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/latest/exports/docs/ble5stack/ble_user_guide/html/oad-secure/bim.html
- TI güncel OAD image header dokümanı: https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/latest/exports/docs/ble5stack/ble_user_guide/doxygen/oad/html/group___o_a_d___i_m_a_g_e___h_e_a_d_e_r.html
- TI SimpleLink CC13xx/CC26xx SDK OAD girişi: https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/8.32.00.07/exports/docs/dmm/dmm_user_guide/html/oad-secure/intro.html
- TI ROM bootloader / CCFG notları: https://www.ti.com/lit/an/swra466e/swra466e.pdf
- Contiki-NG SimpleLink platform dokümanı: https://docs.contiki-ng.org/en/master/doc/platforms/simplelink.html
