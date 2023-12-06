#include <arpa/inet.h>
#include <netinet/in.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

const int BUFFER_SIZE = 1024;
const int PORT = 443;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <server IP address>\n", argv[0]);
    exit(1);
  }

  const char* serverIPAddress = argv[1];

  int socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddress = { .sin_family = AF_INET,
                                       .sin_port = htons(PORT),
                                       .sin_addr.s_addr = inet_addr(serverIPAddress) };

  if (connect(socketFileDescriptor, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) < 0) {
    printf("Error: connect failed\n");
    exit(1);
  }

  SSL_CTX* context = SSL_CTX_new(TLS_method());
  SSL* ssl = SSL_new(context);
  SSL_set_fd(ssl, socketFileDescriptor);
  SSL_connect(ssl);

  char* request = "GET /\r\n\r\n";
  SSL_write(ssl, request, (int)strlen(request));
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);
  SSL_read(ssl, buffer, BUFFER_SIZE - 1);
  printf("Response: %s\n", buffer);

  SSL_shutdown(ssl);
  SSL_free(ssl);
  close(socketFileDescriptor);
  SSL_CTX_free(context);

  return 0;
}
