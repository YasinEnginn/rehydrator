CONTIKI_PROJECT = old-firmware new-firmware
FIRMWARE_GOALS = $(filter old-firmware new-firmware,$(MAKECMDGOALS))

ifneq ($(filter old-firmware,$(FIRMWARE_GOALS)),)
PROJECT_SOURCEFILES += oad_hdr_old.c
endif

ifneq ($(filter new-firmware,$(FIRMWARE_GOALS)),)
PROJECT_SOURCEFILES += oad_hdr.c
endif

ifneq ($(words $(FIRMWARE_GOALS)),1)
ifneq ($(FIRMWARE_GOALS),)
$(error Build only one firmware image at a time)
endif
endif

all:
	@echo "Build one OAD image at a time:"
	@echo "  make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware"
	@echo "  make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware"
	@false

DEBUG = 1
CONTIKI ?= ../..

# Contiki-NG uses a global LDSCRIPT variable, so build the two OAD images
# separately. Each linker script reserves 0x100 bytes for the TI OAD header
# and places reset vectors at the prgEntry address used by the BIM header.
#
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
include $(CONTIKI)/Makefile.include
