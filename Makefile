# == Toolchain ==
CC = gcc

# == Flags ==
CFLAGS  = -Wall -Wextra -g -std=c99 -DLOG_USE_COLOR -DUNITY_OUTPUT_COLOR -DUNITY_INCLUDE_DOUBLE -DUNITY_DOUBLE_PRECISION=1e-12f
LDFLAGS =

# == Directories ==
SRCDIR          = src
INCDIR          = include
BUILDDIR        = build
LIBDIR          = lib
TESTDIR         = tests
UNITYDIR        = $(LIBDIR)/Unity
LOGLIBDIR       = $(LIBDIR)/log.c/src
UNITYMEMORYDIR  = $(UNITYDIR)/extras/memory
UNITYFIXTUREDIR = $(UNITYDIR)/extras/fixture

# == Target Definitions ==
TARGET     = $(BUILDDIR)/main
TARGETTEST = $(BUILDDIR)/test

# == Sources ==
SRC = $(wildcard $(SRCDIR)/*.c) $(wildcard $(LOGLIBDIR)/*.c)
SRCTEST = $(filter-out $(SRCDIR)/main.c, $(SRC))
SRCTESTS = $(wildcard $(TESTDIR)/*.c) \
		   $(wildcard $(UNITYDIR)/src/*.c) \
		   $(wildcard $(UNITYFIXTUREDIR)/src/*.c) \
		   $(wildcard $(UNITYMEMORYDIR)/src/*.c)

# == Object files ==
OBJ = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))

# == Includes ==
INCLUDES     = -I$(INCDIR) -I/usr/include -I$(LOGLIBDIR)
TESTINCLUDES = $(INCLUDES) -I$(UNITYDIR)/src -I$(UNITYFIXTUREDIR)/src -I$(UNITYMEMORYDIR)/src

# == Targets ==
all: main test

run: main
	./$(BUILDDIR)/main

main: $(BUILDDIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# == Directory Setup ==
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# == Unit Testing ==
test: $(BUILDDIR)
	$(CC) $(CFLAGS) $(TESTINCLUDES) $(SRCTEST) $(SRCTESTS) -o $(TARGETTEST)

run-tests: test
	./$(TARGETTEST)

# == Clean ==
clean:
	rm -rf $(BUILDDIR) $(TARGET)

# == Stats ==
cloc:
	cloc $(SRCDIR) $(INCDIR) $(TESTDIR)

.PHONY: all clean test tests cloc
