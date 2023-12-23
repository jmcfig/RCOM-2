#include "url_parser.h"

int decodeURL(char * webAddress, URLComponents * resourceDetails) {
    char * protocol = strtok(webAddress, "/");
    char * credentials = strtok(NULL, "/");
    char * resourceSpecifier = strtok(NULL, "");

    if (protocol == NULL || credentials == NULL || resourceSpecifier == NULL) {
        fprintf(stderr, "Error: The URL provided is not valid!\n");
        return -1;
    }

    if (containsCredentials(credentials)) {
        char * authPart = strtok(credentials, "@");
        char * hostPart = strtok(NULL, "@");

        char * userId = strtok(authPart, ":");
        char * userPassword = strtok(NULL, ":");

        strncpy(resourceDetails->username, userId ? userId : "default", sizeof(resourceDetails->username) - 1);
        strncpy(resourceDetails->secret, userPassword ? userPassword : "pass", sizeof(resourceDetails->secret) - 1);
        strncpy(resourceDetails->server, hostPart, sizeof(resourceDetails->server) - 1);
    }
    else {
        strncpy(resourceDetails->username, "anonymous", sizeof(resourceDetails->username) - 1);
        strncpy(resourceDetails->secret, "pass", sizeof(resourceDetails->secret) - 1);
        strncpy(resourceDetails->server, credentials, sizeof(resourceDetails->server) - 1);
    }

    char * filename = extractFileName(resourceSpecifier);
    strncpy(resourceDetails->resource, resourceSpecifier, sizeof(resourceDetails->resource) - 1);
    strncpy(resourceDetails->resource_name, filename, sizeof(resourceDetails->resource_name) - 1);

    if (!strcmp(resourceDetails->server, "") || !strcmp(resourceDetails->resource, "")) {
        fprintf(stderr, "Error: The URL does not contain complete details!\n");
        return -1;
    }

    struct hostent * hostEntity;

    if ((hostEntity = gethostbyname(resourceDetails->server)) == NULL) {  
        herror("gethostbyname");
        return -1;
    }

    strncpy(resourceDetails->server_name, hostEntity->h_name, sizeof(resourceDetails->server_name) - 1);
    strncpy(resourceDetails->server_ip, inet_ntoa(*((struct in_addr *)hostEntity->h_addr)), sizeof(resourceDetails->server_ip) - 1);

    printf("\nUsername: %s\n", resourceDetails->username);
    printf("Secret: %s\n", resourceDetails->secret);
    printf("Server: %s\n", resourceDetails->server);
    printf("Resource Path: %s\n", resourceDetails->resource);
    printf("Resource Name: %s\n", resourceDetails->resource_name);
    printf("Server Name: %s\n", resourceDetails->server_name);
    printf("Server IP: %s\n\n", resourceDetails->server_ip);

    return 0;
}

int containsCredentials(char * urlString) {
    return strchr(urlString, '@') != NULL;
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

