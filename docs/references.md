# References

Bu dosya, kullanılan dış kaynakları ve proje içi dokümantasyon referanslarını tek
yerde toplar. Dış bağlantılarda mümkün olduğunca resmi üretici veya proje
dokümanları tercih edilmiştir.

## Beklenen Çıktılar

| Beklenen çıktı | Repodaki karşılığı |
| --- | --- |
| Flash ve RAM kullanımını gösteren bellek tablosu | [flash-layout.md](flash-layout.md) |
| Boot zincirini gösteren kısa akış diyagramı | [flash-layout.md](flash-layout.md) |
| CCFG alanının neden kritik olduğu | [flash-layout.md](flash-layout.md), [uniflash-guide.md](uniflash-guide.md), [bim-onchip-notes.md](bim-onchip-notes.md) |
| İkinci firmware kaydı için yerleşim stratejisi | [flash-layout.md](flash-layout.md) |
| Tam firmware değiştirme için neden bootloader gerektiği | [flash-layout.md](flash-layout.md) |
| ELF/section doğrulaması | [build-and-verification.md](build-and-verification.md) |
| Cihaza yüklenecek dosyaların üretimi | [build-and-verification.md](build-and-verification.md) |
| Terimler sözlüğü | [glossary.md](glossary.md) |

## TI Cihaz ve SDK Kaynakları

| Kaynak | Kullanım amacı |
| --- | --- |
| [TI CC1352R ürün sayfası](https://www.ti.com/product/CC1352R) | Cihaz ailesi, teknik dokümanlar ve destek bağlantıları. |
| [TI CC1352R datasheet](https://www.ti.com/lit/ds/symlink/cc1352r.pdf) | Cihaz özellikleri, bellek ve donanım bilgileri. |
| [CC13x2/CC26x2 SimpleLink Wireless MCU Technical Reference Manual](https://www.ti.com/lit/ug/swcu185g/swcu185g.pdf) | Platformun flash, boot, CCFG ve çevre birimleri için ayrıntılı referansı. |
| [SimpleLink Low Power SDK](https://www.ti.com/tool/SIMPLELINK-CC13XX-CC26XX-SDK) | TI SDK, örnekler, BIM/OAD dosyaları ve geliştirme akışı. |
| [TI UniFlash](https://www.ti.com/tool/UNIFLASH) | Cihaza HEX/BIN programlamak için kullanılan araç. |

## TI BIM / OAD Kaynakları

| Kaynak | Kullanım amacı |
| --- | --- |
| [Boot Image Manager documentation](https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/latest/exports/docs/ble5stack/ble_user_guide/html/oad-secure/bim.html) | BIM'in OAD image header analiz etmesi, boot sırası ve CCFG/linker ilişkisi. |
| [OAD overview for CC13xx/CC26xx](https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/6.10.00.29/exports/docs/ble5stack/ble_user_guide/html/ble-stack-5.x-guide/ble5-oad-index-cc13xx_cc26xx.html) | OAD image header, flash layout, BIM ve `oad_image_tool` kavramları. |
| [OAD application notes](https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/8.31.00.11/exports/docs/ble5stack/ble_user_guide/html/ble-stack-oad/oad-application.html) | Image header alanları ve OAD doğrulama akışına dair uygulama notları. |
| [OAD profile documentation](https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/8.32.00.07/exports/docs/ble5stack/ble_user_guide/html/ble-stack-oad/oad-profile.html) | OAD protokol davranışı ve hata/uyumluluk durumları. |
| [OAD image header Doxygen](https://software-dl.ti.com/simplelink/esd/simplelink_cc13xx_cc26xx_sdk/latest/exports/docs/ble5stack/ble_user_guide/doxygen/oad/html/group___o_a_d___i_m_a_g_e___h_e_a_d_e_r.html) | Header struct alanları ve sabitleri için doğrudan referans. |
| [TI ROM bootloader / CCFG application note](https://www.ti.com/lit/an/swra466e/swra466e.pdf) | ROM bootloader ve CCFG davranışını anlamak için ek kaynak. |

## Contiki-NG Kaynakları

| Kaynak | Kullanım amacı |
| --- | --- |
| [Contiki-NG SimpleLink platform](https://docs.contiki-ng.org/en/master/doc/platforms/simplelink.html) | `TARGET=simplelink`, CCS entegrasyonu ve platform bilgileri. |
| [Contiki-NG platforms index](https://docs.contiki-ng.org/en/master/doc/platforms/index.html) | SimpleLink platformunun Contiki-NG içindeki yeri. |

## Build ve Binary Araç Kaynakları

| Kaynak | Kullanım amacı |
| --- | --- |
| [GNU `objcopy`](https://www.sourceware.org/binutils/docs/binutils/objcopy.html) | ELF çıktısından Intel HEX ve raw BIN üretme mantığı. |
| [GNU `readelf`](https://www.sourceware.org/binutils/docs/binutils/readelf.html) | ELF section, symbol ve header bilgilerini okuma. |
| [GNU `objdump`](https://www.sourceware.org/binutils/docs/binutils/objdump.html) | Section içeriğini dump edip OAD header alanlarını inceleme. |
| [Keil Intel HEX file format](https://www.keil.com/support/docs/1584/_hlp_hexfile.htm) | HEX dosyalarının neden adres bilgisi taşıdığını açıklamak için format referansı. |

## Görsel ve Markdown Kaynakları

| Kaynak | Kullanım amacı |
| --- | --- |
| [GitHub Markdown diagrams](https://docs.github.com/en/enterprise-cloud%40latest/get-started/writing-on-github/working-with-advanced-formatting/creating-diagrams) | GitHub üzerinde Mermaid diyagramlarının nasıl render edildiği. |
| [Mermaid flowchart syntax](https://mermaid.ai/open-source/syntax/flowchart.html) | `flash-layout.md` içindeki boot akışı diyagramını yazmak için. |
| [Mermaid introduction](https://mermaid.ai/docs/mermaid-oss/intro/index.html) | Mermaid'in Markdown tabanlı diyagram üretim yaklaşımı. |
