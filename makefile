CC = gcc
CFLAGS = -Wall -s -std=c99 -O3 -I. -lm -march=native -mtune=native -static

TARGET = sg1gen
SRC = main.c sg1.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)