#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h> 
#include <netdb.h> 
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include "parse.h"
#include "sockets.h"
#include "macros.h"

// Download a file using FTP
int ftpDownloadFile(URLComponents * urlComponents);

// Transfer the file data from the FTP server
int ftpTransferData(int dataSocket, const char * filename);
