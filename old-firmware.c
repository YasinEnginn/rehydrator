#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include "hw3_layout.h"

#define LOG_MODULE "old-fw"
#define LOG_LEVEL LOG_LEVEL_INFO
#define U(x) ((unsigned long)(x))
#define END_ADDR(base, size) U((base) + (size) - 1UL)

PROCESS(old_firmware_process, "BIL304 HW3 old firmware");
AUTOSTART_PROCESSES(&old_firmware_process);

PROCESS_THREAD(old_firmware_process, ev, data)
{
  static struct etimer timer;
  static unsigned long tick;

  PROCESS_BEGIN();

  LOG_INFO("old firmware running, version=0x%08lx\n"
           "active app: 0x%08lx-0x%08lx\n"
           "new image storage: 0x%08lx-0x%08lx\n"
           "metadata: 0x%08lx-0x%08lx, ccfg: 0x%08lx-0x%08lx\n",
           U(HW3_OLD_FW_VERSION),
           U(HW3_ACTIVE_APP_BASE), END_ADDR(HW3_ACTIVE_APP_BASE, HW3_ACTIVE_APP_SIZE),
           U(HW3_NEW_IMAGE_BASE), END_ADDR(HW3_NEW_IMAGE_BASE, HW3_NEW_IMAGE_SIZE),
           U(HW3_UPDATE_METADATA_BASE), END_ADDR(HW3_UPDATE_METADATA_BASE, HW3_UPDATE_METADATA_SIZE),
           U(HW3_CCFG_BASE), END_ADDR(HW3_CCFG_BASE, HW3_CCFG_SIZE));

  etimer_set(&timer, CLOCK_SECOND * 4);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    tick++;
    LOG_INFO("old firmware heartbeat %lu; waiting for preloaded new image\n",
             tick);
    leds_toggle(LEDS_GREEN);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
