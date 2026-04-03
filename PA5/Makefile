CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = proxy

all: $(TARGET)

$(TARGET): Proxy.c
	$(CC) $(CFLAGS) -o $(TARGET) proxy.c

clean:
	rm -f $(TARGET)
