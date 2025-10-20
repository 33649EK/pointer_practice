CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c17 -MMD -MP
LDFLAGS =
LDLIBS  =

BUILD   = builds
ASM_DIR = assembly-files

# Add program basenames here (each must have a matching .c)
PROGS   = singly_linked_list

EXES    = $(addprefix $(BUILD)/,$(PROGS))
OBJS    = $(addprefix $(BUILD)/,$(addsuffix .o,$(PROGS)))
DEPS    = $(OBJS:.o=.d)

# Assembly outputs (one .s per prog.c)
ASMS    = $(addprefix $(ASM_DIR)/,$(addsuffix .s,$(PROGS)))

.PHONY: all clean assembly
all: $(EXES)

# Ensure output directories exist
$(BUILD) $(ASM_DIR):
	mkdir -p $@

# Compile: source.c -> builds/source.o (with auto header deps)
$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link: builds/prog from its own object only
$(BUILD)/%: $(BUILD)/%.o | $(BUILD)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# --- Assembly generation ---

# `make assembly` builds all .s files into assembly/
assembly: $(ASMS)

# Emit AT&T .s from each source (use -masm=intel if you prefer Intel syntax)
$(ASM_DIR)/%.s: %.c | $(ASM_DIR)
	$(CC) $(CFLAGS) -S $< -o $@

clean:
	rm -rf $(BUILD) $(ASM_DIR)

-include $(DEPS)
