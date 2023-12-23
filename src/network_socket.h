#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h> 
#include <netdb.h> 
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#define CONNECTION_SUCCESSFUL 1
#define CONNECTION_FAILED 0

typedef struct {
    char serverIP[20];
    int serverPort;
} FTPPassiveModeData;

typedef struct {
    char serverResponse[1024];
    int statusCode;
} FTPServerResponse;

// Initializes the FTP connection
int establishFTPConnection(const char * serverIP, int serverPort);

// Authenticates the user
int authenticateUser(int ftpSocket, const char * userName, const char * userPassword);

// Enters passive mode
int enterPassiveMode(int ftpSocket, FTPPassiveModeData * passiveData);

// Sends an FTP command
int executeFTPCommand(int ftpSocket, const char * ftpCommand, int argumentPresent, const char * commandArguments);

// Writes a message to the FTP socket
int writeToFTPSocket(int ftpSocket, const char * message);

// Reads a response from the FTP server
int readFTPServerResponse(int ftpSocket, FTPServerResponse * response);

// Constructs an FTP command
void constructFTPCommand(const char * ftpCommand, int argumentPresent, const char * commandArguments, char * constructedCommand);

// Verifies the server's response code
int validateResponseCode(int ftpSocket, int expectedCode);

// Checks if the server's response code is one of the expected codes
int validateMultipleResponseCodes(int ftpSocket, const int expectedCodes[], int numberOfCodes);
