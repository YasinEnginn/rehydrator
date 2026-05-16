#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include "oad_layout.h"

#define LOG_MODULE "new-fw"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(new_firmware_process, "BIL304 OAD user firmware");
AUTOSTART_PROCESSES(&new_firmware_process);

PROCESS_THREAD(new_firmware_process, ev, data)
{
  static struct etimer timer;
  static unsigned long tick;

  PROCESS_BEGIN();

  LOG_INFO("[NEW-FW] update image is live, version=0x%08lx\n",
           (unsigned long)OAD_USER_FW_VERSION);
  LOG_INFO("[NEW-FW] BIM jumped to user entry 0x%08lx in slot 0x%08lx\n",
           (unsigned long)OAD_USER_ENTRY,
           (unsigned long)OAD_USER_IMAGE_BASE);

  etimer_set(&timer, CLOCK_SECOND * 2);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    tick++;
    LOG_INFO("[NEW-FW] heartbeat %lu; upgraded firmware is running\n", tick);
    leds_toggle(LEDS_RED);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
