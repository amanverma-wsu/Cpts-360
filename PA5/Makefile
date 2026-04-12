# Compiler
CC = gcc

# Flags
CFLAGS = -Wall -Wextra -g

# Output executable
TARGET = proxy

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): Proxy.c
	$(CC) $(CFLAGS) -o $(TARGET) Proxy.c

# Clean build files
clean:
	rm -f $(TARGET)

# Rebuild everything
rebuild: clean all
