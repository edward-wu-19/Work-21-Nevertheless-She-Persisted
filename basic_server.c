#include "pipe_networking.h"


int main() {

    int to_client;
    int from_client;
    
    char line[LEN];
    int i;

    while (1){
      from_client = server_handshake( &to_client );;

    // int pToC = open("pToC", O_RDONLY);
    // int cToP = open("cToP", O_WRONLY);

    char* tmp = line;
    i = 0;

      // printf("A\n");

      while (1){
          if (read(from_client, line, LEN) == 0){
            break;
          };

          // char* tmp = line;
          // int i = 0;
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