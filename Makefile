CONTIKI_PROJECT = old-firmware new-firmware

all: $(CONTIKI_PROJECT)

DEBUG = 1
CONTIKI ?= ../..

include $(CONTIKI)/Makefile.include
