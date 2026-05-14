CONTIKI_PROJECT = old-firmware new-firmware
all: $(CONTIKI_PROJECT)
DEBUG = 1
CONTIKI ?= ../..

new-firmware_src += oad_hdr.c
old-firmware_src += oad_hdr_old.c

include $(CONTIKI)/Makefile.include
new-firmware_LDSCRIPT = new-firmware.ld
