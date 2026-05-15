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
	@echo "Create device upload files:"
	@echo "  make TARGET=simplelink BOARD=sensortag/cc1352r1 upload-files"
	@false

DEBUG = 1
TARGET ?= simplelink
BOARD ?= sensortag/cc1352r1
CONTIKI ?= ../..
OBJCOPY ?= arm-none-eabi-objcopy
MKDIR_P ?= mkdir -p
UPLOAD_DIR ?= upload

BUILD_DIR = build/$(TARGET)/$(BOARD)

.PHONY: upload-files new-firmware-upload-files old-firmware-upload-files

upload-files: old-firmware-upload-files new-firmware-upload-files

new-firmware-upload-files:
	$(MAKE) TARGET=$(TARGET) BOARD=$(BOARD) LDSCRIPT=new-firmware.ld new-firmware
	$(MKDIR_P) $(UPLOAD_DIR)
	@elf="$$(find "$(BUILD_DIR)" -type f -name "new-firmware.$(TARGET)" | head -n 1)"; \
	if [ -z "$$elf" ]; then \
	  echo "Could not find new-firmware.$(TARGET) under $(BUILD_DIR)"; \
	  exit 1; \
	fi; \
	echo "Using $$elf"; \
	$(OBJCOPY) -O ihex "$$elf" "$(UPLOAD_DIR)/new-firmware.hex"; \
	$(OBJCOPY) -O binary --gap-fill 0xFF "$$elf" "$(UPLOAD_DIR)/new-firmware.bin"; \
	echo "Created $(UPLOAD_DIR)/new-firmware.hex and $(UPLOAD_DIR)/new-firmware.bin"

old-firmware-upload-files:
	$(MAKE) TARGET=$(TARGET) BOARD=$(BOARD) LDSCRIPT=old-firmware.ld old-firmware
	$(MKDIR_P) $(UPLOAD_DIR)
	@elf="$$(find "$(BUILD_DIR)" -type f -name "old-firmware.$(TARGET)" | head -n 1)"; \
	if [ -z "$$elf" ]; then \
	  echo "Could not find old-firmware.$(TARGET) under $(BUILD_DIR)"; \
	  exit 1; \
	fi; \
	echo "Using $$elf"; \
	$(OBJCOPY) -O ihex "$$elf" "$(UPLOAD_DIR)/old-firmware.hex"; \
	$(OBJCOPY) -O binary --gap-fill 0xFF "$$elf" "$(UPLOAD_DIR)/old-firmware.bin"; \
	echo "Created $(UPLOAD_DIR)/old-firmware.hex and $(UPLOAD_DIR)/old-firmware.bin"

# Contiki-NG uses a global LDSCRIPT variable, so build the two OAD images
# separately. Each linker script reserves 0x100 bytes for the TI OAD header
# and places reset vectors at the prgEntry address used by the BIM header.
#
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=old-firmware.ld old-firmware
# make TARGET=simplelink BOARD=sensortag/cc1352r1 LDSCRIPT=new-firmware.ld new-firmware
# make TARGET=simplelink BOARD=sensortag/cc1352r1 upload-files
include $(CONTIKI)/Makefile.include
