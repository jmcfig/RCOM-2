#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "download.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: ./bin/download ftp://[<username>:<password>@]<hostname>/<file-path>\n");
        exit(EXIT_FAILURE);
    }

    URLComponents ftpURL;

    if (decodeURL(argv[1], &ftpURL) < 0) {
        fprintf(stderr, "Error: Invalid FTP URL format!\n");
        exit(EXIT_FAILURE);
    }

    if (ftpDownloadFile(&ftpURL) < 0) {
        fprintf(stderr, "Error: Failed to download the FTP file!\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

