CONTIKI_PROJECT = old-firmware new-firmware
all: $(CONTIKI_PROJECT)
DEBUG = 1
CONTIKI ?= ../..

new-firmware_src += oad_hdr.c
old-firmware_src += oad_hdr_old.c

# Contiki-NG uses LDSCRIPT, not a per-project *_LDSCRIPT variable.
# Build the staged image with:
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
include $(CONTIKI)/Makefile.include
