
# --- Architecture du projet
BUILD_DIR = build
SRC_DIR = src
INC_DIR = include

# --- Exécutable cible
TARGET = spiderbot.elf



# --- Configuration des outils
LOG = stm32f4.log
GDB = gdb-multiarch
# GDB = gdb
#GDB = gdb-multiarch
GDB_FLAGS=-command scripts/gdbinit


# --- Configuration du compilateur
ARCH=arm-none-eabi-
#ARCH=arm-linux-gnueabihf-
CC=$(ARCH)gcc
AS=$(ARCH)as
OBJCOPY=$(ARCH)objcopy
CFLAGS += -g3 -Wall -Wextra -O0 -MMD -std=c17
ASFLAGS += -g
CFLAGS += \
	-Tscripts/link.ld \
	-mthumb \
	-mfloat-abi=hard \
	-mlittle-endian \
	-mcpu=cortex-m4 \
	-mfpu=fpv4-sp-d16 \
	-I $(INC_DIR)


LDFLAGS = \
	-static \
	-nostdinc -nostdlib \
	-nostartfiles



# --- Récupération des fichiers
SRC_C := $(wildcard $(SRC_DIR)/*.c)
SRC_S := $(wildcard $(SRC_DIR)/*.s)
OBJ_FILES := \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_C)) \
	$(patsubst $(SRC_DIR)/%.s,$(BUILD_DIR)/%.o,$(SRC_S))
DEP_FILES := $(OBJ_FILES:.o=.d)


.PHONY: all clean openocd log debug

all: $(TARGET)

# --- Génération de l'exécutable

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s | $(BUILD_DIR)
	$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# --- Outils

clean:
	rm -f $(LOG) $(TARGET)
	rm -rf $(BUILD_DIR)

-include $(DEP_FILES)

openocd:
	-rm -f $(LOG)
	openocd -f scripts/openocd.cfg

log:
	tail -f $(LOG)

debug: $(TARGET)
	$(GDB) $(GDB_FLAGS) $(TARGET)