# UniFlash Guide

Bu dosya cihaz programlama sırasını ve HEX/BIN kullanım kararlarını açıklar.

## Temel Kural

Full chip erase yapılmamalıdır; BIM/CCFG bölgesi korunmalıdır.

TI UniFlash indirme sayfası:

```text
https://www.ti.com/tool/UNIFLASH
```

## Gerekli HEX Dosyaları

| Dosya | Kaynak | Not |
| --- | --- | --- |
| `../bim_onchip/Debug_unsecure/bim_onchip.hex` | TI BIM projesi | BIM ve CCFG alanını içerir. |
| `upload/old-firmware.hex` | Bu projenin `upload-files` hedefi | Eski ve ilk çalıştırılacak persistent fallback firmware imajıdır. |
| `upload/new-firmware.hex` | Bu projenin `upload-files` hedefi | Yeni user/update firmware imajıdır. |

## Yükleme Senaryosu

```text
1. BIM             -> 0x00056000 - 0x00057FFF
2. old-firmware    -> 0x00030000 - 0x00051FFF
3. Reset           -> old-firmware çalışır
4. new-firmware    -> 0x00000000 - 0x0002FFFF
5. Reset           -> new-firmware çalışır
```

## HEX ve BIN Farkı

HEX dosyalarıyla yükleme yapılırsa adresler dosyanın içinde bulunduğu için
programlama aracında ek başlangıç adresi verilmesi gerekmez.

BIN dosyasıyla yükleme yapılacaksa adresler ayrıca verilmelidir:

```text
upload/old-firmware.bin -> 0x00030000
upload/new-firmware.bin -> 0x00000000
```

Bu projede `.hex` kullanmak daha güvenlidir; çünkü adres bilgisi dosyanın içinde
taşınır ve yanlış başlangıç adresi girme riski azalır.

## UniFlash ile Adım Adım

1. Önce yükleme dosyalarını üret:

```sh
make TARGET=simplelink BOARD=sensortag/cc1352r1 upload-files
```

2. Kartı USB/debugger üzerinden bilgisayara bağla.

3. UniFlash'i aç ve yeni oturumda cihaz olarak `CC1352R1F3` seç. Bağlantı
   arayüzü olarak kart üzerindeki debugger görünüyorsa `XDS110` seçilebilir.

4. Erase ayarlarında `Mass Erase`, `Erase Entire Flash` veya benzeri tüm flash'ı
   silen seçenekleri kullanma. Amaç sadece yüklenecek HEX dosyasının kapsadığı
   sektörleri yazmak ve BIM/CCFG alanını yanlışlıkla silmemektir.

5. İlk olarak BIM dosyasını yükle:

```text
../bim_onchip/Debug_unsecure/bim_onchip.hex
```

Bu dosya BIM'i `0x00056000` bölgesine, CCFG bilgisini de flash'ın son kısmına
yerleştirir. Bu adım boot zincirinin başlangıcıdır; cihaz reset sonrası önce
BIM'e girer.

6. İkinci olarak eski firmware dosyasını yükle:

```text
upload/old-firmware.hex
```

Bu dosya `0x00030000` adresindeki persistent fallback slotuna yazılır. Page 0'da
henüz yeni user imaj olmadığı için BIM fallback imajı seçip `0x00030100` entry
adresine atlar.

7. Cihazı resetle ve eski firmware'in çalıştığını kontrol et:

```text
old-firmware çalışırsa yeşil LED heartbeat
```

8. Güncelleme aşamasında yeni firmware dosyasını yükle:

```text
upload/new-firmware.hex
```

Bu dosya `0x00000000` adresindeki user image slotuna yazılır. BIM reset sonrası
önce bu imajı arar ve geçerli görürse `0x00000100` entry adresine atlar.

9. Programlama bittikten sonra cihazı tekrar resetle. Beklenen davranış:

```text
new-firmware çalışırsa kırmızı LED heartbeat
```

## Kısa Sıra

Önerilen UniFlash sırası kısaca şöyledir:

```text
1. bim_onchip.hex
2. upload/old-firmware.hex
3. Reset -> old-firmware
4. upload/new-firmware.hex
5. Reset -> new-firmware
```

İstersen üç HEX dosyasını tek oturumda art arda da yazabilirsin. Bu durumda
reset sonrası doğrudan `new-firmware` çalışır; `old-firmware` fallback olarak
flash'ta kalır.
