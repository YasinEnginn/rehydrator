#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include <stdint.h>
#include <string.h>

#include "hw3_image.h"

#define LOG_MODULE "old-fw"
#define LOG_LEVEL LOG_LEVEL_INFO
#define TO_ULONG(value) ((unsigned long)(value))
#define RANGE_END(base, size) ((base) + (size) - 1UL)

#ifndef HW3_CONF_ENABLE_STAGING_CHECK
#if defined(CONTIKI_TARGET_SIMPLELINK)
#define HW3_CONF_ENABLE_STAGING_CHECK 1
#else
#define HW3_CONF_ENABLE_STAGING_CHECK 0
#endif
#endif

PROCESS(old_firmware_process, "BIL304 HW3 old firmware");
AUTOSTART_PROCESSES(&old_firmware_process);

#if HW3_CONF_ENABLE_STAGING_CHECK
static uint32_t
crc32_update(uint32_t crc, const uint8_t *data, uint32_t len)
{
  uint32_t i;

  while(len-- > 0) {
    crc ^= *data++;
    for(i = 0; i < 8; i++) {
      crc = (crc & 1U) ? ((crc >> 1) ^ 0xedb88320UL) : (crc >> 1);
    }
  }

  return crc;
}

static uint32_t
crc32_calc(const void *data, uint32_t len)
{
  return crc32_update(0xffffffffUL, data, len) ^ 0xffffffffUL;
}

static void
log_staged_image_status(void)
{
  const struct hw3_image_header *header;
  struct hw3_image_header header_copy;
  const uint8_t *payload;
  uint32_t header_crc;
  uint32_t image_crc;

  header = (const struct hw3_image_header *)(uintptr_t)HW3_NEW_IMAGE_BASE;

  if(header->magic != HW3_IMAGE_MAGIC) {
    LOG_INFO("no staged firmware image at 0x%08lx\n",
             TO_ULONG(HW3_NEW_IMAGE_BASE));
    return;
  }

  if(header->header_size != sizeof(*header) ||
     header->header_size != HW3_IMAGE_HEADER_SIZE ||
     header->layout_version != HW3_IMAGE_LAYOUT_VERSION) {
    LOG_INFO("staged image rejected: unsupported header\n");
    return;
  }

  if(header->target != HW3_ACTIVE_APP_BASE) {
    LOG_INFO("staged image rejected: target=0x%08lx expected=0x%08lx\n",
             TO_ULONG(header->target),
             TO_ULONG(HW3_ACTIVE_APP_BASE));
    return;
  }

  if(header->image_size == 0 ||
     header->image_size > HW3_IMAGE_MAX_PAYLOAD_SIZE) {
    LOG_INFO("staged image rejected: invalid size=%lu max=%lu\n",
             TO_ULONG(header->image_size),
             TO_ULONG(HW3_IMAGE_MAX_PAYLOAD_SIZE));
    return;
  }

  memcpy(&header_copy, header, sizeof(header_copy));
  header_copy.header_crc = 0;
  header_crc = crc32_calc(&header_copy, sizeof(header_copy));

  if(header_crc != header->header_crc) {
    LOG_INFO("staged image rejected: header crc=0x%08lx expected=0x%08lx\n",
             TO_ULONG(header_crc),
             TO_ULONG(header->header_crc));
    return;
  }

  payload = (const uint8_t *)(uintptr_t)(HW3_NEW_IMAGE_BASE + header->header_size);
  image_crc = crc32_calc(payload, header->image_size);

  if(image_crc != header->image_crc) {
    LOG_INFO("staged image rejected: image crc=0x%08lx expected=0x%08lx\n",
             TO_ULONG(image_crc),
             TO_ULONG(header->image_crc));
    return;
  }

  LOG_INFO("staged image valid: version=0x%08lx size=%lu target=0x%08lx\n",
           TO_ULONG(header->version),
           TO_ULONG(header->image_size),
           TO_ULONG(header->target));
  LOG_INFO("BIM can activate it by copying staging to active app on reset\n");
}
#else
static void
log_staged_image_status(void)
{
  LOG_INFO("staged image check disabled for this Contiki-NG target\n");
}
#endif

PROCESS_THREAD(old_firmware_process, ev, data)
{
  static struct etimer timer;
  static unsigned long seconds;

  PROCESS_BEGIN();

  LOG_INFO("old firmware running, version=0x%08lx\n",
           TO_ULONG(HW3_OLD_FW_VERSION));
  LOG_INFO("active app: 0x%08lx-0x%08lx\n",
           TO_ULONG(HW3_ACTIVE_APP_BASE),
           TO_ULONG(RANGE_END(HW3_ACTIVE_APP_BASE, HW3_ACTIVE_APP_SIZE)));
  LOG_INFO("new image storage: 0x%08lx-0x%08lx\n",
           TO_ULONG(HW3_NEW_IMAGE_BASE),
           TO_ULONG(RANGE_END(HW3_NEW_IMAGE_BASE, HW3_NEW_IMAGE_SIZE)));
  LOG_INFO("metadata: 0x%08lx-0x%08lx, ccfg: 0x%08lx-0x%08lx\n",
           TO_ULONG(HW3_UPDATE_METADATA_BASE),
           TO_ULONG(RANGE_END(HW3_UPDATE_METADATA_BASE, HW3_UPDATE_METADATA_SIZE)),
           TO_ULONG(HW3_CCFG_BASE),
           TO_ULONG(RANGE_END(HW3_CCFG_BASE, HW3_CCFG_SIZE)));
  log_staged_image_status();

  leds_on(LEDS_GREEN);
  LOG_INFO("green LED on\n");

  etimer_set(&timer, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    seconds++;
    leds_on(LEDS_GREEN);
    LOG_INFO("old firmware: %lu. saniye; green LED on\n", seconds);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
