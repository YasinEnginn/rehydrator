#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include "hw3_image.h"

#define LOG_MODULE "new-fw"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(new_firmware_process, "BIL304 HW3 new firmware");
AUTOSTART_PROCESSES(&new_firmware_process);

PROCESS_THREAD(new_firmware_process, ev, data)
{
  static struct etimer timer;
  static unsigned long tick;

  PROCESS_BEGIN();

  LOG_INFO("new firmware image running, version=0x%08lx\n",
           (unsigned long)HW3_NEW_FW_VERSION);
  LOG_INFO("packaged image is staged at 0x%08lx and targets 0x%08lx\n",
           (unsigned long)HW3_NEW_IMAGE_BASE,
           (unsigned long)HW3_ACTIVE_APP_BASE);

  etimer_set(&timer, CLOCK_SECOND * 2);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    tick++;
    LOG_INFO("new firmware heartbeat %lu\n", tick);
    leds_toggle(LEDS_RED);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
