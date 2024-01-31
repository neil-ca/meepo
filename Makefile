CC = gcc
CFLAGS = -std=c11 -Wall
LIBS = -ledit -lm
SRC = parsing.c mpc.c evaluation.c
TARGET = parsing

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)
