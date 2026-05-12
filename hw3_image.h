#ifndef HW3_IMAGE_H_
#define HW3_IMAGE_H_

#include <stdint.h>

#include "hw3_layout.h"

#define HW3_IMAGE_MAGIC            0x46573352UL
#define HW3_IMAGE_LAYOUT_VERSION   1U
#define HW3_IMAGE_HEADER_SIZE      48U
#define HW3_IMAGE_MAX_PAYLOAD_SIZE (HW3_NEW_IMAGE_SIZE - HW3_IMAGE_HEADER_SIZE)

struct hw3_image_header {
  uint32_t magic;
  uint16_t header_size;
  uint16_t layout_version;
  uint32_t version;
  uint32_t image_size;
  uint32_t image_crc;
  uint32_t target;
  uint32_t flags;
  uint32_t reserved[4];
  uint32_t header_crc;
};

typedef char hw3_image_header_size_must_be_48[
  sizeof(struct hw3_image_header) == HW3_IMAGE_HEADER_SIZE ? 1 : -1
];

#endif /* HW3_IMAGE_H_ */
