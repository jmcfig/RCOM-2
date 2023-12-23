CC = gcc
CFLAGS = -Wall -g
SRC = src/
INCLUDE = include/
BIN = bin/
.PHONY: run
run: $(BIN)/download
$(BIN)/download: $(SRC)/download.c $(SRC)/parse.c $(SRC)/main.c $(SRC)/sockets.c
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean
clean:
	rm -rf $(BIN)/download
