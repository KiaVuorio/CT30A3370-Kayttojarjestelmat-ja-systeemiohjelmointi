CC = gcc
CFLAGS = -Wall -Wextra -g
TARGETS = my-cat my-grep my-zip my-unzip

all: $(TARGETS)

$(TARGETS): %: %.c
	$(CC) $(CCFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)