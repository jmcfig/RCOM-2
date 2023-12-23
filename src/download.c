#include "download.h"

int ftpDownloadFile(URLComponents * urlComponents) {
    // Establish socket connection
    int controlSocket;
    if ((controlSocket = establishFTPConnection(urlComponents->server_ip, 21)) < 0) {
        fprintf(stderr, "Error: Connection initialization failed!\n");
        return -1;
    }

    if (validateResponseCode(controlSocket, FTP_READY) < 0)
        return -1;

    // Perform user authentication
    if (authenticateUser(controlSocket, urlComponents->username, urlComponents->secret) < 0) {
        fprintf(stderr, "Error: Authentication failed!\n");
        return -1;
    }

    // Enter passive mode
    FTPPassiveModeData passiveData;
    if (enterPassiveMode(controlSocket, &passiveData) < 0) {
        fprintf(stderr, "Error: Failed to set passive mode!\n");
        return -1;
    }

    int dataSocket;
    if ((dataSocket = establishFTPConnection(passiveData.serverIP, passiveData.serverPort)) < 0) {
        fprintf(stderr, "Error: Data connection initialization failed!\n");
        return -1;
    }

    // Request file
    if (executeFTPCommand(controlSocket, FTP_RETR, CONNECTION_SUCCESSFUL, urlComponents->resource) < 0) {
        fprintf(stderr, "Error: Failed to send RETR command!\n");
        return -1;
    }

    if (validateResponseCode(controlSocket, FTP_READY_FOR_TRANSFER) < 0)
        return -1;

    // Transfer file
    if (ftpTransferData(dataSocket, urlComponents->resource_name) < 0) {
        fprintf(stderr, "Error: File transfer failed!\n");
        return -1;
    }

    if (validateResponseCode(controlSocket, FTP_TRANSFER_COMPLETED) < 0)
        return -1;

    close(controlSocket);
    close(dataSocket);

    return 0;
}

int ftpTransferData(int dataSocket, const char * filename) {
    int fileDescriptor;
    if ((fileDescriptor = open(filename, O_WRONLY | O_CREAT, 0777)) < 0) {
        fprintf(stderr, "Error: Could not open file for writing!\n");
        return -1;
    }

    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = read(dataSocket, buffer, sizeof(buffer))) > 0) {
        if (write(fileDescriptor, buffer, bytesRead) < 0) {
            fprintf(stderr, "Error: Failed to write to file!\n");
            close(fileDescriptor);
            return -1;
        }
    }

    close(fileDescriptor);
    return 0;
}
