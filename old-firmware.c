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
  LOG_INFO("active/persistent OAD slot: 0x%08lx-0x%08lx entry=0x%08lx\n",
           TO_ULONG(OAD_ACTIVE_IMAGE_BASE),
           TO_ULONG(RANGE_END(OAD_ACTIVE_IMAGE_BASE, OAD_ACTIVE_IMAGE_SIZE)),
           TO_ULONG(OAD_ACTIVE_ENTRY));
  LOG_INFO("user OAD slot: 0x%08lx-0x%08lx entry=0x%08lx\n",
           TO_ULONG(OAD_STAGING_IMAGE_BASE),
           TO_ULONG(RANGE_END(OAD_STAGING_IMAGE_BASE, OAD_STAGING_IMAGE_SIZE)),
           TO_ULONG(OAD_STAGING_ENTRY));
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

  LOG_INFO("persistent firmware running, version=0x%08lx\n",
           TO_ULONG(OAD_PERSISTENT_FW_VERSION));
  LOG_INFO("TI BIM selected the persistent fallback image\n");
  log_oad_layout();

  leds_on(LEDS_GREEN);
  LOG_INFO("green LED on\n");

  etimer_set(&timer, CLOCK_SECOND);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER && data == &timer);
    seconds++;
    leds_on(LEDS_GREEN);
    LOG_INFO("persistent firmware heartbeat %lu; green LED on\n", seconds);
    etimer_reset(&timer);
  }

  PROCESS_END();
}
