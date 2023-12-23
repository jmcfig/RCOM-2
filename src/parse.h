#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h> 
#include <netdb.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

// Structure for storing URL components
typedef struct {
    char server[512];
    char username[256];
    char secret[256];
    char resource[512];
    char resource_name[512];
    char server_name[512];
    char server_ip[20];
} URLComponents;

// Function prototypes
int decodeURL(char * inputURL, URLComponents * components);
char * extractFileName(char * resourcePath);
int containsCredentials(char * urlString);
