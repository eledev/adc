
TARGET = main

#######################################
# binaries
#######################################
PREFIX = avr-

ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc 
CP = $(GCC_PATH)/$(PREFIX)objcopy
AR = $(GCC_PATH)/$(PREFIX)ar
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


DEBUG = 0


# Micro controller unit
MCU = -mmcu=atmega8



######################################
# source
######################################
# C sources
C_SOURCES =  \
src/$(TARGET).c 


# Includes directories
INCLUDES_DIRS =  \
-Isrc


# Sources directories
SOURCES_DIRS =  \
src/



# Build path
BUILD_DIR = build

vpath %.c $(sort $(dir $(C_SOURCES)))


# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
#OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -g -Os -MMD -MP -MF"$(@:%.o=%.d)"  $(MCU) $(INCLUDES_DIRS) -c  $< -o $@



$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) -g  $(MCU) $(OBJECTS) -o $@
	$(SZ) $@


$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(CP) -j .text -j .data -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(CP) -O binary -S $< $@


$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# clean up
#######################################
install:
	avrdude -p m8 -c usbasp -P usb -U flash:w:"$(BUILD_DIR)/$(TARGET).hex":i


#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)
