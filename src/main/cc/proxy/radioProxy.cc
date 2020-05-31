#include <memory>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "err.h"
#include <memory>
#define BUFFER_SIZE 1024

#include "audio-stream-sinks/audioStreamSinkFactory.h"
#include "response-parser/responseParser.h"

int main(int argc, char *argv[]) {

  int sock;
  struct addrinfo addr_hints;
  struct addrinfo *addr_result;

  int i, err;
  char buffer[BUFFER_SIZE];
  ssize_t rcv_len;

  char *host = "waw02-03.ic.smcdn.pl";
  char *port = "8000";

  // 'converting' host/port in string to struct addrinfo
  memset(&addr_hints, 0, sizeof(struct addrinfo));
  addr_hints.ai_family = AF_INET; // IPv4
  addr_hints.ai_socktype = SOCK_STREAM;
  addr_hints.ai_protocol = IPPROTO_TCP;
  err = getaddrinfo(host, port, &addr_hints, &addr_result);
  if (err == EAI_SYSTEM) { // system error
    printf("getaddrinfo: %s", gai_strerror(err));
    return 1;
  } else if (err != 0) { // other error (host not found, etc.)
    printf("getaddrinfo: %s", gai_strerror(err));
    return 1;
  }

  // initialize socket according to getaddrinfo results
  sock = socket(addr_result->ai_family, addr_result->ai_socktype,
    addr_result->ai_protocol);
  if (sock < 0) {
    printf("socket");
    return 1;
  }

  // connect socket to the server
  if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0) {
    printf("connect");

    return 1;
  }

  freeaddrinfo(addr_result);

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, argv[0]);

  char *message = "GET /t043-1.mp3 HTTP/1.1\r\n"
                  "Host: waw02-03.ic.smcdn.pl:8000\r\n"
                  "User-Agent: curl/7.64.1\r\n"
                  "Accept: */*\r\n"
                  "Icy-MetaData: 1\r\n"
                  "Connection: close\r\n\r\n";

  FILE *file = fdopen(sock, "r+");
  if (fprintf(file, "%s", message) < 0) {
    printf("partial / failed write");
    return 1;
  }

//  if (write(sock, message, strlen(message)) != strlen(message)) {
//    printf("partial / failed write");
//    return 1;
//  }
  char *line = NULL;
  size_t len = 0;
  getline(&line, &len, file);

  responseResolver->parseStatusLine(line);

  while(responseResolver->hasHeadersEnded()) {
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, file);

    responseResolver->parseHeader(line);
  }

  while(true) {
    memset(buffer, 0, sizeof(buffer));
    rcv_len = read(sock, buffer, sizeof(buffer) - 1);

//    char *line = NULL;
//    size_t len = 0;
//    if (getline(&line, &len, file) == -1) {
//      break;
//    }
    responseResolver->parseBody(std::string(buffer, rcv_len));
//    responseResolver->parseHeader(std::string(buffer, rcv_len));
//    std::cout << len << ": " << strlen(line)<< "-> " << std::string(line) << "->>>>" << (std::string(line)  == "\r\n") << std::endl;

    if (rcv_len == 0) {
      break;
    }
//
    if (rcv_len < 0) {
      printf("read");
      return 1;
    }
//
//    std::string a(buffer, rcv_len);
//    std::cout << a;

  }
  fclose(file);
  (void) close(sock); // socket would be closed anyway when the program ends

  return 0;
}