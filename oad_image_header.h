#ifndef OAD_IMAGE_HEADER_H_
#define OAD_IMAGE_HEADER_H_

#include <stdint.h>

#define OAD_IMG_TYPE_PERSISTENT_APP       0
#define OAD_IMG_TYPE_APPSTACKLIB          7

#define OAD_WIRELESS_TECH_PROPRF          0xFFBF

#ifndef OAD_IMG_ID_VAL
#if defined(DeviceFamily_CC13X2) || defined(DeviceFamily_CC13X2X7) || \
    defined(DeviceFamily_CC13X1) || defined(DeviceFamily_CC13X2_V1) || \
    defined(DeviceFamily_CC13X2_V2)
#define OAD_IMG_ID_VAL                    {'C', 'C', '1', '3', 'x', '2', 'R', '1'}
#elif defined(DeviceFamily_CC26X2) || defined(DeviceFamily_CC26X2_V2) || \
      defined(DeviceFamily_CC26X2X7) || defined(DeviceFamily_CC26X1)
#define OAD_IMG_ID_VAL                    {'C', 'C', '2', '6', 'x', '2', 'R', '1'}
#else
#define OAD_IMG_ID_VAL                    {'C', 'C', '1', '3', 'x', '2', 'R', '1'}
#endif
#endif

#define OAD_IMG_ID_LEN                    8
#define OAD_SW_VER_LEN                    4

#define IMG_PAYLOAD_SEG_ID                0x01

#define BIM_VER                           0x03
#define META_VER                          0x01

#define DEFAULT_STATE                     0xFF
#define INVALID_ADDR                      0xFFFFFFFFUL
#define INVALID_LEN                       INVALID_ADDR
#define DEFAULT_CRC                       0xFFFFFFFFUL

typedef struct __attribute__((packed)) {
  uint8_t   imgID[OAD_IMG_ID_LEN];
  uint32_t  crc32;
  uint8_t   bimVer;
  uint8_t   metaVer;
  uint16_t  techType;
  uint8_t   imgCpStat;
  uint8_t   crcStat;
  uint8_t   imgType;
  uint8_t   imgNo;
  uint32_t  imgVld;
  uint32_t  len;
  uint32_t  prgEntry;
  uint8_t   softVer[OAD_SW_VER_LEN];
  uint32_t  imgEndAddr;
  uint16_t  hdrLen;
  uint16_t  rfu;
} imgFixedHdr_t;

typedef struct __attribute__((packed)) {
  uint8_t   segTypeImg;
  uint16_t  wirelessTech;
  uint8_t   rfu;
  uint32_t  imgSegLen;
  uint32_t  startAddr;
} imgPayloadSeg_t;

typedef struct __attribute__((packed)) {
   imgFixedHdr_t   fixedHdr;
   imgPayloadSeg_t imgPayload;
} imgHdr_t;

#define OAD_IMG_HDR_LEN                   sizeof(imgFixedHdr_t)
#define OAD_IMG_FULL_HDR_LEN              sizeof(imgHdr_t)

#endif
