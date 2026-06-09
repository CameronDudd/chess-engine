CC = gcc

MAJOR	:= 0
MINOR	:= 0
PATCH	:= 1
VERSION	:= v$(MAJOR).$(MINOR).$(PATCH)

BUILD 		?= debug
TARGET_BASE 	:= main
TARGET		:= $(TARGET_BASE)_$(VERSION)_$(BUILD)

SRC_DIR		:= src
BUILD_DIR	:= build/$(BUILD)
INCLUDE_DIR	:= include

SRC		:= $(wildcard $(SRC_DIR)/*.c)

OBJ		:= $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

LOG_LIB_DIR	:= lib/log.c/src
LOG_LIB_SRC	:= $(wildcard $(LOG_LIB_DIR)/*.c)
LOG_LIB_OBJ	:= $(LOG_LIB_SRC:$(LOG_LIB_DIR)/%.c=$(BUILD_DIR)/log/%.o)

C_FLAGS_COMMON	:= -Wall -Wextra -std=c11 \
		   -MMD -MP \
		   -I$(INCLUDE_DIR) \
		   -I$(LOG_LIB_DIR) \
		   -DLOG_USE_COLOR
LDFLAGS		:=

ifeq ($(BUILD),release)
	CFLAGS := $(C_FLAGS_COMMON) -O2 -DNDEBUG
else
	CFLAGS := $(C_FLAGS_COMMON) -O0 -g3 -DDEBUG
endif

DEP := $(OBJ:.o=.d) $(LOG_LIB_OBJ:.o=.d)

-include $(DEP)

all: $(TARGET) compile_commands.json

run: $(TARGET)
	./$(TARGET)

init:
	git submodule update --init --recursive

$(TARGET): $(OBJ) $(LOG_LIB_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/log/%.o: $(LOG_LIB_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

cloc:
	cloc --md $(SRC_DIR) $(INCLUDE_DIR)

format:
	clang-format -i $(SRC)

check:
	clang-format --dry-run --Werror $(SRC)
	clang-tidy -p build $(SRC) --warnings-as-errors=*

info:
	@echo "Target: $(TARGET)"
	@echo "Compiler: $(CC)"
	@echo "Build type: $(BUILD)"

compile_commands.json: $(SRC) Makefile
	@bear -- make $(TARGET)

.PHONY: all init run clean cloc format check info compile_commands.json
