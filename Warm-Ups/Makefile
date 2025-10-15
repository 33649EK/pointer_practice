CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c17
BUILD   = builds
TARGET  = $(BUILD)/integer_rotation
OBJS    = $(BUILD)/integer_rotation.o

.PHONY: all clean
all: $(TARGET)

# Ensure build directory exists
$(BUILD):
	mkdir -p $@

# Compile: source.c -> builds/source.o
$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the final executable in builds/
$(TARGET): $(OBJS) | $(BUILD)
	$(CC) $^ -o $@

clean:
	rm -rf $(BUILD)
