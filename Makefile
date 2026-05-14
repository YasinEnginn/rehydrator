CONTIKI_PROJECT = old-firmware new-firmware
all: $(CONTIKI_PROJECT)
DEBUG = 1
CONTIKI ?= ../..

new-firmware_src += oad_hdr.c
old-firmware_src += oad_hdr_old.c

# Contiki-NG uses a global LDSCRIPT variable, so build the two OAD images
# separately. Each linker script reserves 0x100 bytes for the TI OAD header
# and places reset vectors at the prgEntry address used by the BIM header.
#
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
include $(CONTIKI)/Makefile.include
