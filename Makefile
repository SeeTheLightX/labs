CC = gcc
CFLAGS = -std=c23 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Wvla -Werror -g3 -O1 -fno-omit-frame-pointer
SANITIZERS = -fsanitize=address,undefined
TARGET = lab1
OBJECTS = main.o stats.o

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(SANITIZERS) -o $(TARGET)

main.o: main.c stats.h record.h node.h
	$(CC) $(CFLAGS) $(SANITIZERS) -c main.c -o main.o

stats.o: stats.c stats.h
	$(CC) $(CFLAGS) $(SANITIZERS) -c stats.c -o stats.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)