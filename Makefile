CC = gcc
CFLAGS = -Wall -g
SRC = src/
INCLUDE = include/
BIN = bin/
.PHONY: run
run: $(BIN)/download
$(BIN)/download: $(SRC)/ftp_fetcher.c $(SRC)/url_parser.c $(SRC)/ftp_downloader.c $(SRC)/network_socket.c
	$(CC) $(CFLAGS) -o $@ $^
.PHONY: clean
clean:
	rm -rf $(BIN)/download
