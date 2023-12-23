#include "url_parser.h"

int decodeURL(char * inputURL, URLComponents * components) {
    char * protocol = strtok(inputURL, "/");
    char * credentials = strtok(NULL, "/");
    char * resourcePath = strtok(NULL, "");

    if (protocol == NULL || credentials == NULL || resourcePath == NULL) {
        fprintf(stderr, "Invalid URL format!\n");
        return -1;
    }

    if (containsCredentials(credentials)) {
        char * loginDetails = strtok(credentials, "@");
        char * serverAddr = strtok(NULL, "@");

        char * userName = strtok(loginDetails, ":");
        char * userPass = strtok(NULL, ":");

        strcpy(components->username, userName ? userName : "default");
        strcpy(components->secret, userPass ? userPass : "default");
        strcpy(components->server, serverAddr);
    }
    else {
        strcpy(components->username, "guest");
        strcpy(components->secret, "none");
        strcpy(components->server, credentials);
    }

    char * fileName = extractFileName(resourcePath);
    strcpy(components->resource, resourcePath);
    strcpy(components->resource_name, fileName);

    if (!strcmp(components->server, "") || !strcmp(components->resource, "")) {
        fprintf(stderr, "Incomplete URL details!\n");
        return -1;
    }

    struct hostent * hostInfo;
    if ((hostInfo = gethostbyname(components->server)) == NULL) {  
        herror("resolve host");
        return -1;
    }

    strcpy(components->server_name, hostInfo->h_name);
    strcpy(components->server_ip, inet_ntoa(*((struct in_addr *)hostInfo->h_addr)));

    printf("\nUsername: %s\n", components->username);
    printf("Password: %s\n", components->secret);
    printf("Server: %s\n", components->server);
    printf("Resource Path: %s\n", components->resource);
    printf("Resource Name: %s\n", components->resource_name);
    printf("Server Name: %s\n", components->server_name);
    printf("Server IP: %s\n\n", components->server_ip);

    return 0;
}

char * extractFileName(char * resourcePath) {
    char * fileName = resourcePath;
    char * lastToken = resourcePath;
    for (char * p = resourcePath; *p; p++) {
        if (*p == '/' || *p == '\\' || *p == ':') {
            lastToken = p + 1;
        }
    }
    int tokenLength = strlen(lastToken);
    fileName = (char *)malloc(tokenLength);  
    if (fileName != NULL) {
        strncpy(fileName, lastToken, tokenLength - 1);
        fileName[tokenLength - 1] = '\0'; 
    }
    return fileName;
}



int containsCredentials(char * urlString) {
    return strchr(urlString, '@') != NULL;
}