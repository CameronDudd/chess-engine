CC = gcc

MAJOR		:= 0
MINOR		:= 0
PATCH		:= 3
PRE_RELEASE	:= alpha
VERSION	:= v$(MAJOR).$(MINOR).$(PATCH)-$(PRE_RELEASE)

BUILD 		?= debug
TARGET_BASE 	:= main
TARGET		:= $(TARGET_BASE)_$(VERSION)_$(BUILD)
TEST_TARGET	:= test_$(VERSION)_$(BUILD)

SRC_DIR		:= src
BUILD_DIR	:= build/$(BUILD)
INCLUDE_DIR	:= include

SRC		:= $(wildcard $(SRC_DIR)/*.c)

OBJ		:= $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

TEST_DIR	:= tests
TEST_SRC	:= $(wildcard $(TEST_DIR)/*.c)
TEST_OBJ	:= $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_DIR)/tests/%.o)

LOG_LIB_DIR	:= lib/log.c/src
LOG_LIB_SRC	:= $(wildcard $(LOG_LIB_DIR)/*.c)
LOG_LIB_OBJ	:= $(LOG_LIB_SRC:$(LOG_LIB_DIR)/%.c=$(BUILD_DIR)/log/%.o)

UNITY_LIB_DIR		:= lib/Unity
UNITY_LIB_SRC_DIRS	:= $(UNITY_LIB_DIR)/src $(UNITY_LIB_DIR)/extras/memory/src $(UNITY_LIB_DIR)/extras/fixture/src
UNITY_LIB_SRCS		:= $(foreach dir,$(UNITY_LIB_SRC_DIRS),$(wildcard $(dir)/*.c))
UNITY_LIB_OBJS		:= $(patsubst lib/Unity/%.c, $(BUILD_DIR)/unity/%.o, $(UNITY_LIB_SRCS))

ARGPARSE_LIB_DIR	:= lib/argparse
ARGPARSE_LIB		:= $(ARGPARSE_LIB_DIR)/libargparse.a

C_FLAGS_COMMON	:= -Wall -Wextra -std=c11 \
		   -MMD -MP \
		   -I$(INCLUDE_DIR) \
		   -I$(LOG_LIB_DIR) \
		   -I$(ARGPARSE_LIB_DIR) \
		   $(foreach dir,$(UNITY_LIB_SRC_DIRS),-I$(dir)) \
		   -DLOG_USE_COLOR \
		   -DTARGET=\"$(TARGET)\" \
	           -DUNITY_OUTPUT_COLOR

LDFLAGS		:= $(ARGPARSE_LIB)

ifeq ($(BUILD),release)
	CFLAGS := $(C_FLAGS_COMMON) -O3 -march=native -DNDEBUG
else
	CFLAGS := $(C_FLAGS_COMMON) -O0 -g3 -DDEBUG
endif

DEP := $(OBJ:.o=.d) $(LOG_LIB_OBJ:.o=.d)

-include $(DEP)

init:
	git submodule update --init --recursive
	$(MAKE) -C lib/argparse

all: $(TARGET)

perft: $(TARGET)
	./$(TARGET) perft -h

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TARGET): $(OBJ) $(LOG_LIB_OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/log/%.o: $(LOG_LIB_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_TARGET): $(filter-out $(BUILD_DIR)/main.o, $(OBJ)) $(LOG_LIB_OBJ) $(TEST_OBJ) $(UNITY_LIB_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/tests/%.o: $(TEST_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/unity/%.o: lib/Unity/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

cloc:
	cloc --md $(SRC_DIR) $(INCLUDE_DIR) $(TEST_DIR)

format:
	clang-format -i $(SRC)

check:
	clang-format --dry-run --Werror $(SRC)
	clang-tidy -p build $(SRC) --warnings-as-errors=*

info:
	@echo "Target: $(TARGET)"
	@echo "Compiler: $(CC)"
	@echo "Build type: $(BUILD)"

compile_commands.json: clean $(SRC) Makefile
	@bear -- make $(TARGET)

.PHONY: all init test perft clean cloc format check info compile_commands.json
