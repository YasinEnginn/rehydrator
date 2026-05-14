#include <stdint.h>
typedef struct __attribute__((packed)) {
    uint8_t  imgID[8];
    uint32_t crc32;
    uint8_t  bimVer;
    uint8_t  metaVer;
    uint16_t techType;
    uint8_t  imgCpStat;
    uint8_t  crcStat;
    uint8_t  imgType;
    uint8_t  imgNo;
    uint32_t imgVld;
    uint32_t len;
    uint32_t prgEntry;
    uint8_t  softVer[4];
    uint32_t imgEndAddr;
    uint16_t hdrLen;
    uint16_t rfu;
} imgFixedHdr_t;
typedef struct __attribute__((packed)) {
    uint8_t  segTypeImg;
    uint16_t wirelessTech;
    uint8_t  rfu;
    uint32_t imgSegLen;
    uint32_t startAddr;
} imgPayloadSeg_t;
typedef struct __attribute__((packed)) {
    imgFixedHdr_t   fixedHdr;
    imgPayloadSeg_t imgPayload;
} imgHdr_t;
const imgHdr_t _imgHdr __attribute__((section(".image_header"), used)) = {
    .fixedHdr = {
        .imgID      = {'C','C','1','3','x','2','R','1'},
        .crc32      = 0xFFFFFFFF,
        .bimVer     = 0x3,
        .metaVer    = 0x1,
        .techType   = 0xFFBF,
        .imgCpStat  = 0xFF,
        .crcStat    = 0xFF,
        .imgType    = 0x7,
        .imgNo      = 0x0,
        .imgVld     = 0xFFFFFFFF,
        .len        = 0xFFFFFFFF,
        .prgEntry   = 0x00000000,
        .softVer    = {0x00, 0x00, 0x01, 0x00},
        .imgEndAddr = 0xFFFFFFFF,
        .hdrLen     = sizeof(imgHdr_t),
        .rfu        = 0xFFFF,
    },
    .imgPayload = {
        .segTypeImg   = 0x1,
        .wirelessTech = 0xFFBF,
        .rfu          = 0xFF,
        .imgSegLen    = 0xFFFFFFFF,
        .startAddr    = 0x00000000,
    }
};
