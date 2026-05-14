#include <stdint.h>

#include <ti/common/cc26xx/oad/oad_image_header.h>

#include "oad_layout.h"

const imgHdr_t _imgHdr __attribute__((section(".image_header"), used)) = {
    .fixedHdr = {
        .imgID      = OAD_IMG_ID_VAL,
        .crc32      = DEFAULT_CRC,
        .bimVer     = BIM_VER,
        .metaVer    = META_VER,
        .techType   = OAD_WIRELESS_TECH_PROPRF,
        .imgCpStat  = DEFAULT_STATE,
        .crcStat    = DEFAULT_STATE,
        .imgType    = OAD_IMG_TYPE_PERSISTENT_APP,
        .imgNo      = 0x0,
        .imgVld     = INVALID_ADDR,
        .len        = INVALID_LEN,
        .prgEntry   = OAD_ACTIVE_ENTRY,
        .softVer    = {0x00, 0x00, 0x01, 0x00},
        .imgEndAddr = INVALID_ADDR,
        .hdrLen     = OAD_IMG_FULL_HDR_LEN,
        .rfu        = 0xFFFF,
    },
    .imgPayload = {
        .segTypeImg   = IMG_PAYLOAD_SEG_ID,
        .wirelessTech = OAD_WIRELESS_TECH_PROPRF,
        .rfu          = DEFAULT_STATE,
        .imgSegLen    = INVALID_LEN,
        .startAddr    = OAD_ACTIVE_IMAGE_BASE,
    }
};
