#include "sockets.h"

int establishFTPConnection(const char * serverIP, int serverPort) {
    int socketFD;
    struct sockaddr_in serverAddress;

    bzero((char*)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);
    serverAddress.sin_port = htons(serverPort);

    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error: Socket creation failed!\n");
        return CONNECTION_FAILED;
    }

    if (connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        fprintf(stderr, "Error: Unable to connect to server!\n");
        return CONNECTION_FAILED;
    }

    return socketFD;
}

int authenticateUser(int ftpSocket, const char * userName, const char * userPassword) {
    if (executeFTPCommand(ftpSocket, "USER", CONNECTION_SUCCESSFUL, userName) < 0) {
        fprintf(stderr, "Error: USER command failed!\n");
        return CONNECTION_FAILED;
    }

    int validResponses[2] = {331, 230};

    if (validateMultipleResponseCodes(ftpSocket, validResponses, 2) < 0)
        return CONNECTION_FAILED;

    if (executeFTPCommand(ftpSocket, "PASS", CONNECTION_SUCCESSFUL, userPassword) < 0) {
        fprintf(stderr, "Error: PASS command failed!\n");
        return CONNECTION_FAILED;
    }

    if (validateResponseCode(ftpSocket, 230) < 0)
        return CONNECTION_FAILED;

    return CONNECTION_SUCCESSFUL;
}

int enterPassiveMode(int ftpSocket, FTPPassiveModeData * passiveData) {
    FTPServerResponse ftpResponse;
    memset(&ftpResponse, 0, sizeof(FTPServerResponse));

    if (executeFTPCommand(ftpSocket, "PASV", CONNECTION_FAILED, NULL) < 0) {
        fprintf(stderr, "Error: PASV command failed!\n");
        return CONNECTION_FAILED;
    }

    if (readFTPServerResponse(ftpSocket, &ftpResponse) < 0) {
        return CONNECTION_FAILED;
    }

    if (ftpResponse.statusCode != 227) {
        fprintf(stderr, "Error: Unexpected response code for PASV!\n");
        return CONNECTION_FAILED;
    }

    // Parse PASV Response
    strtok(ftpResponse.serverResponse, "(");
    char * ipPortStr = strtok(NULL, ")");

    int ipPart[4], portPart[2];
    sscanf(ipPortStr, "%d,%d,%d,%d,%d,%d", &ipPart[0], &ipPart[1], &ipPart[2], &ipPart[3], &portPart[0], &portPart[1]);

    passiveData->serverPort = portPart[0] * 256 + portPart[1];
    sprintf(passiveData->serverIP, "%d.%d.%d.%d", ipPart[0], ipPart[1], ipPart[2], ipPart[3]);

    return CONNECTION_SUCCESSFUL;
}

int executeFTPCommand(int ftpSocket, const char * ftpCommand, int argumentPresent, const char * commandArguments) {
    char constructedCommand[256];
    constructFTPCommand(ftpCommand, argumentPresent, commandArguments, constructedCommand);

    if (writeToFTPSocket(ftpSocket, constructedCommand) < 0) {
        return CONNECTION_FAILED;
    }

    return CONNECTION_SUCCESSFUL;
}

int writeToFTPSocket(int ftpSocket, const char * message) {
    int bytesSent;
    if ((bytesSent = write(ftpSocket, message, strlen(message))) != strlen(message)) {
        fprintf(stderr, "Error: Failed to write message to FTP socket!\n");
        return CONNECTION_FAILED;
    }

    printf("> %s", message);

    return bytesSent;
}

int readFTPServerResponse(int ftpSocket, FTPServerResponse * response) {
    FILE * socketStream = fdopen(ftpSocket, "r");
    char * buffer;
    size_t bytesRead = 0;
    int totalBytesRead = 0;

    while (getline(&buffer, &bytesRead, socketStream) > 0) {
        strncat(response->serverResponse, buffer, bytesRead - 1);
        totalBytesRead += bytesRead;

        if (buffer[3] == ' ') {
            sscanf(buffer, "%d", &response->statusCode);
            break;
        }
    }

    free(buffer);

    printf("< %s", response->serverResponse);

    return totalBytesRead;
}

void constructFTPCommand(const char * ftpCommand, int argumentPresent, const char * commandArguments, char * constructedCommand) {
    strcpy(constructedCommand, ftpCommand);
    if (argumentPresent == CONNECTION_SUCCESSFUL) {
        strcat(constructedCommand, " ");
        strcat(constructedCommand, commandArguments);
    }
    strcat(constructedCommand, "\r\n");
}

int validateResponseCode(int ftpSocket, int expectedCode) {
    static FTPServerResponse response;
    memset(&response, 0, sizeof(FTPServerResponse));

    if (readFTPServerResponse(ftpSocket, &response) < 0) {
        return CONNECTION_FAILED;
    }

    if (response.statusCode != expectedCode) {
        fprintf(stderr, "Error: Unexpected response code received!\n");
        return CONNECTION_FAILED;
    }

    return CONNECTION_SUCCESSFUL;
}

int validateMultipleResponseCodes(int ftpSocket, const int expectedCodes[], int numberOfCodes) {
    static FTPServerResponse response;
    memset(&response, 0, sizeof(FTPServerResponse));

    if (readFTPServerResponse(ftpSocket, &response) < 0) {
        return CONNECTION_FAILED;
    }

    for (int i = 0; i < numberOfCodes; i++) {
        if (response.statusCode == expectedCodes[i])
            return CONNECTION_SUCCESSFUL;
    }

    fprintf(stderr, "Error: Unexpected response code received!\n");
    return CONNECTION_FAILED;
}
