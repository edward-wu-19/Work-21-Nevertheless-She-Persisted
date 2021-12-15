#include "pipe_networking.h"


int main() {

    int to_client;
    int from_client;

    while (1){
      from_client = server_handshake( &to_client );;

    // int pToC = open("pToC", O_RDONLY);
    // int cToP = open("cToP", O_WRONLY);

      char line[LEN];

      // printf("A\n");

      while (1){
          read(from_client, line, LEN);

          char* tmp = line;
          int i = 0;
          while (i < LEN){
              *tmp = toupper(*tmp);
              tmp++;
              i++;
          }

          write(to_client, line, LEN);
      }

    }

    return 0;
}