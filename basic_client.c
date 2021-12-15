#include "pipe_networking.h"


int main() {

    int to_server;
    int from_server; // client is the interface server is the worker, child

    from_server = client_handshake( &to_server );

    // int to_server = open("pToC", O_WRONLY);
    // int from_server = open("cToP", O_RDONLY);

    char line[LEN];

    while (1){
      printf("Please enter your message:\n");
        fgets(line, LEN, stdin);

        write(to_server, line, LEN);

        read(from_server, line, LEN);
        printf("The child says: \n%s\n", line);
    }

    return 0;
}