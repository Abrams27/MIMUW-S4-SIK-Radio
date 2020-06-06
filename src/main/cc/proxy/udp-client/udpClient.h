#ifndef MIMUW_S4_SIK_RADIO_UDPCLIENT_H
#define MIMUW_S4_SIK_RADIO_UDPCLIENT_H

#include <string>
#include "udpClient.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


class UdpClient {
public:
  UdpClient(int port);
  ~UdpClient();

  void sendMessage(const std::string &message, uint16_t port, uint32_t address);
  void sendMessage(const std::string &message, struct sockaddr_in clientAddress);
  sockaddr_in getLatestClientAddress();
  std::string readMessage(size_t size);
  bool hasPreviousOperationBeenInterrupted();
  bool hasPreviousOperationTimeouted();

private:
  int socketId;
  struct sockaddr_in latestClientAddress;

  bool hasPreviousOperationBeenInterruptedFlag = false;
  bool hasPreviousOperationTimeoutedFlag = false;

  void initSocket();

  void checkErrnoAndUpdateInterruptFlagOrExit();
  void checkErrnoAndUpdateInterruptFlagAndTimeoutFlagOrExit();
};


#endif //MIMUW_S4_SIK_RADIO_UDPCLIENT_H
