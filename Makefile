CC = gcc
CFLAGS = -Wall -g
SRC = src/
INCLUDE = include/
BIN = bin/
.PHONY: run
run: $(BIN)/download
$(BIN)/download: $(SRC)/download.c $(SRC)/args.c $(SRC)/file.c $(SRC)/socket.c
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean
clean:
	rm -rf $(BIN)/download
