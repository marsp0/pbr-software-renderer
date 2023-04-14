# https://en.wikipedia.org/wiki/Makefile

SRC_DIR         := ./src
OBJ_DIR         := ./out

EXE             := renderer
SRC_FILES       := $(shell find $(SRC_DIR) -name "*.c")
SRC_NAMES       := $(filter-out test%.c, $(notdir $(SRC_FILES)))
OBJ_FILES       := $(SRC_NAMES:%.c=$(OBJ_DIR)/%.o)

TEST_EXE        := test_renderer
TEST_SRC_NAMES  := $(filter-out main.c, $(notdir $(SRC_FILES)))
TEST_OBJ_FILES  := $(TEST_SRC_NAMES:%.c=$(OBJ_DIR)/%.o)

LDFLAGS         := -lX11 -lm
space           :=
VPATH           := $(subst $(space),:,$(shell find . -type d))
GCCFLAGS        := -std=c17 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wnull-dereference -Wunused -Wconversion -Wno-pointer-sign -g3 -pg


.PHONY: all renderer test
all: out/$(EXE) out/$(TEST_EXE)
renderer: out/$(EXE)
test: out/$(TEST_EXE)
test: GCCFLAGS += --coverage

out/$(EXE): $(OBJ_FILES)
	gcc $(GCCFLAGS) $(OBJ_FILES) -o $@ $(LDFLAGS)

out/$(TEST_EXE): $(TEST_OBJ_FILES)
	gcc $(GCCFLAGS) $(TEST_OBJ_FILES) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	gcc $(GCCFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@rm -f out/* && echo "[CL]  out/"
