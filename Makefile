# Makefile for the custom shell

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
LDFLAGS =

# Source files
SRCS := $(wildcard src/*.c)

# Object files
OBJS := $(SRCS:.c=.o)

# Executable name
TARGET = my_shell

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

src/%.o: src/%.c src/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)