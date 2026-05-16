#include "contiki.h"
#include "sys/etimer.h"
#include "sys/log.h"
#include "dev/leds.h"

#include "oad_layout.h"

#define LOG_MODULE "persist-fw"
#define LOG_LEVEL LOG_LEVEL_INFO
#define TO_ULONG(value) ((unsigned long)(value))
#define RANGE_END(base, size) ((base) + (size) - 1UL)

PROCESS(old_firmware_process, "BIL304 OAD persistent firmware");
AUTOSTART_PROCESSES(&old_firmware_process);

static void
log_oad_layout(void)
{
  LOG_INFO("user OAD slot: 0x%08lx-0x%08lx entry=0x%08lx\n",
           TO_ULONG(OAD_USER_IMAGE_BASE),
           TO_ULONG(RANGE_END(OAD_USER_IMAGE_BASE, OAD_USER_IMAGE_SIZE)),
           TO_ULONG(OAD_USER_ENTRY));
  LOG_INFO("persistent fallback slot: 0x%08lx-0x%08lx entry=0x%08lx\n",
           TO_ULONG(OAD_PERSISTENT_IMAGE_BASE),
           TO_ULONG(RANGE_END(OAD_PERSISTENT_IMAGE_BASE, OAD_PERSISTENT_IMAGE_SIZE)),
           TO_ULONG(OAD_PERSISTENT_ENTRY));
  LOG_INFO("metadata: 0x%08lx-0x%08lx, recovery: 0x%08lx-0x%08lx\n",
           TO_ULONG(OAD_UPDATE_METADATA_BASE),
           TO_ULONG(RANGE_END(OAD_UPDATE_METADATA_BASE, OAD_UPDATE_METADATA_SIZE)),
           TO_ULONG(OAD_RECOVERY_BASE),
           TO_ULONG(RANGE_END(OAD_RECOVERY_BASE, OAD_RECOVERY_SIZE)));
  LOG_INFO("BIM/CCFG: 0x%08lx-0x%08lx\n",
           TO_ULONG(OAD_BIM_CCFG_BASE),
           TO_ULONG(RANGE_END(OAD_BIM_CCFG_BASE, OAD_BIM_CCFG_SIZE)));
}

PROCESS_THREAD(old_firmware_process, ev, data)
{
  static struct etimer timer;
  static unsigned long seconds;

  PROCESS_BEGIN();

  LOG_INFO("[OLD-FW] fallback image awake, version=0x%08lx\n",
           TO_ULONG(OAD_PERSISTENT_FW_VERSION));
  LOG_INFO("[OLD-FW] BIM picked the backup slot; safe boot is OK\n");
  log_oad_layout();

  leds_on(LEDS_GREEN);
  LOG_INFO("[OLD-FW] green LED requested, serial log is the source of truth\n");

  etimer_set(&timer, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    seconds++;
    leds_on(LEDS_GREEN);
    LOG_INFO("[OLD-FW] heartbeat %lu; waiting in fallback mode\n", seconds);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
