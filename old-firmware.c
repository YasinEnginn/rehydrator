#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include "hw3_layout.h"

#define LOG_MODULE "old-fw"
#define LOG_LEVEL LOG_LEVEL_INFO
#define TO_ULONG(value) ((unsigned long)(value))
#define RANGE_END(base, size) ((base) + (size) - 1UL)

PROCESS(old_firmware_process, "BIL304 HW3 old firmware");
AUTOSTART_PROCESSES(&old_firmware_process);

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
