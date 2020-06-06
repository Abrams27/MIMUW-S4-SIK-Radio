#include <iostream>
#include <thread>
#include <chrono>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#define DISCOVER 1
#define IAM 2
#define KEEPALIVE 3
#define AUDIO 4
#define METADATA 5

#define BLOCK_SIZE 65000

struct MessageHeader {
  uint16_t typeNetworkOrder;
  uint16_t lengthNetworkOrder;
};

struct MessageHeaderData {
  uint16_t type;
  uint16_t length;
};

MessageHeaderData parseProxyCommunicationHeader(MessageHeader data) {
  MessageHeaderData result;
  result.type = ntohs(data.typeNetworkOrder);
  result.length = ntohs(data.lengthNetworkOrder);

  return result;
}

MessageHeader encryptProxyCommunicationHeader(MessageHeaderData data) {
  MessageHeader result;
  result.typeNetworkOrder = htons(data.type);
  result.lengthNetworkOrder = htons(data.length);

  return result;
}

MessageHeader constructMessageHeader(uint16_t type, uint16_t length) {
  MessageHeaderData data;
  data.type = type;
  data.length = length;

  MessageHeader result = encryptProxyCommunicationHeader(data);
  return result;
}

void keepAlive(int sock, struct sockaddr_in& my_address, int sleeptime) {
  MessageHeader keepHeader = constructMessageHeader(KEEPALIVE, 0);

  while (true) {
    // std::cerr << "keep\n";
    socklen_t l = (socklen_t) sizeof(my_address);
    ssize_t s = sendto(sock, &keepHeader, 4, 0, (struct sockaddr *) &my_address, l);

    std::this_thread::sleep_for(std::chrono::seconds(sleeptime));
  }
}

int main(int argc, char *argv[]) {
  struct addrinfo addr_hints;
  struct addrinfo *addr_result;

  int sock;
  int i, flags, sflags;
  char buffer[BLOCK_SIZE];
  size_t len;
  ssize_t snd_len, rcv_len;
  struct sockaddr_in my_address;
  struct sockaddr_in srvr_address;
  socklen_t rcva_len;

  if (argc != 4) {
    std::cerr << "host port sleeptime\n";
    return 1;
  }

  int sleeptime = std::stoi(argv[3]);

  // 'converting' host/port in string to struct addrinfo
  (void) memset(&addr_hints, 0, sizeof(struct addrinfo));
  addr_hints.ai_family = AF_INET; // IPv4
  addr_hints.ai_socktype = SOCK_DGRAM;
  addr_hints.ai_protocol = IPPROTO_UDP;
  addr_hints.ai_flags = 0;
  addr_hints.ai_addrlen = 0;
  addr_hints.ai_addr = NULL;
  addr_hints.ai_canonname = NULL;
  addr_hints.ai_next = NULL;
  if (getaddrinfo(argv[1], NULL, &addr_hints, &addr_result) != 0) {
    std::cerr << "getaddrinfo\n";
    return 1;
  }

  my_address.sin_family = AF_INET; // IPv4
  my_address.sin_addr.s_addr =
      ((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
  my_address.sin_port = htons((uint16_t) atoi(argv[2])); // port from the command line

  freeaddrinfo(addr_result);

  sock = socket(PF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    std::cerr << "socket\n";
    return 1;
  }

  /* uaktywnienie rozgłaszania (ang. broadcast) */ 
  int optval = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
    exit(1);

  // /* ustawienie TTL dla datagramów rozsyłanych do grupy */ 
  // optval = 4;
  // if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
  //   exit(1);

  MessageHeader discoverHeader = constructMessageHeader(DISCOVER, 0);

  rcva_len = (socklen_t) sizeof(my_address);
  snd_len = sendto(sock, &discoverHeader, 4, 0, (struct sockaddr *) &my_address, rcva_len);
  
  MessageHeader header;
  MessageHeaderData data;

  std::thread t(keepAlive, sock, std::ref(my_address), sleeptime);
  std::ofstream ofstream = std::ofstream("/dev/stdout", std::ios_base::binary | std::ios_base::out);

  while (true) {
    rcv_len = recvfrom(sock, buffer, BLOCK_SIZE, 0, (struct sockaddr *) &srvr_address, &rcva_len);
    // std::cerr << rcv_len << " bytes received.\n";

    if (rcv_len == -1) {
      std::cerr << "recvfrom error\n";
      continue;
    }

    memcpy(&header, buffer, 4);
    data = parseProxyCommunicationHeader(header);
    // std::cerr << "receive: " << data.type << " " << data.length << '\n';

    if (data.type == 4){
      ofstream << std::string(buffer + 4, data.length);
    } else if (data.type == 2) {
      std::cerr << "IAM: " <<  std::string(buffer + 4, data.length);
    } else if (data.type == 6) {
      std::cerr << "Metadane: " <<  std::string(buffer + 4, data.length);
    }
  }

  ofstream.close();


  if (close(sock) == -1) { //very rare errors can occur here, but then
    return 1;
  }

  return 0;
}