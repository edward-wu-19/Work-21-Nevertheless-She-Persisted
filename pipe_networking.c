#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
    
    printf("Creating well known pipe\n");
    int sToCerr = mkfifo(WKP, 0644);
    if (sToCerr == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Opening well known pipe to receive messages\n");
    int from_client = open(WKP, O_RDONLY);
    if (from_client == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Receiving messages\n");
    char message[HANDSHAKE_BUFFER_SIZE];
    int foo = read(from_client, message, HANDSHAKE_BUFFER_SIZE);
    if (foo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Removing well known pipe\n");
    remove(WKP);

    printf("Connecting to client\n");
    *to_client = open(message, O_WRONLY);
    if (*to_client == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }
    
    printf("Step 2: Acknowledgement\n");
    int goo = write(*to_client, ACK, sizeof(ACK));
    if (goo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Receiving message\n");
    int hoo = read(from_client, message, sizeof(message));
    if (hoo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Verifying message\n");
    if (strcmp(message, ACK) == 0){
        printf("Three ways handshake achieved!\n");
    }
    else{
        printf("Three way handshake failed!\n");
    }

    return from_client;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

    char pidp[1024];
    sprintf(pidp, "%d", getpid());

    printf("Making secret connection\n");
    mkfifo(pidp, 0644);

    printf("Opening well known pipe\n");
    *to_server = open(WKP, O_WRONLY);
    if (*to_server == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Step 1: Sending name to server\n");
    int foo = write(*to_server, pidp, HANDSHAKE_BUFFER_SIZE);
    if (foo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Receiving server message\n");
    int from_server = open(pidp, O_RDONLY);
    if (from_server == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Reading server message\n");
    char message[HANDSHAKE_BUFFER_SIZE];
    int goo = read(from_server, message, HANDSHAKE_BUFFER_SIZE);
    if (goo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    printf("Removing secret pipe\n");
    remove(pidp);

    printf("Step 3: Writing response to server\n");
    int hoo = write(*to_server, ACK, sizeof(ACK));
    if (hoo == -1){
        printf("Error: %s\n", strerror(errno));
        return 0;
    }

    return from_server;
}