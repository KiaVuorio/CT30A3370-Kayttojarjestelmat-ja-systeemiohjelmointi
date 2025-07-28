CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = reverse

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) reverse.c

clean:
	rm -f $(TARGET)