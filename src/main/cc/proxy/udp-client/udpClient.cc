#include "udpClient.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <errno.h>

UdpClient::UdpClient(int port) {
  struct sockaddr_in serverAddress;

  initSocket();
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);

  if (bind(socketId, (struct sockaddr *) &serverAddress, (socklen_t) sizeof(serverAddress)) < 0) {
    exit(1);
  }
}

void UdpClient::initSocket() {
  socketId = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketId < 0) {
    exit(1);
  }
}


void UdpClient::sendMessage(const std::string &message, uint16_t port, uint32_t address) {
  struct sockaddr_in clientAddress;

  clientAddress.sin_family = AF_INET;
  clientAddress.sin_addr.s_addr = address;
  clientAddress.sin_port = port;

  hasPreviousOperationBeenInterruptedFlag = false;
  hasPreviousOperationTimeoutedFlag = false;

  if (sendto(socketId, &message[0], message.size(), 0, (struct sockaddr *) &clientAddress, (socklen_t) sizeof(clientAddress)) < 0) {
    checkErrnoAndUpdateInterruptFlagOrExit();
  }
}

void UdpClient::sendMessage(const std::string &message, struct sockaddr_in clientAddress) {
  hasPreviousOperationBeenInterruptedFlag = false;
  hasPreviousOperationTimeoutedFlag = false;

  if (sendto(socketId, &message[0], message.size(), 0, (struct sockaddr *) &clientAddress, (socklen_t) sizeof(clientAddress)) < 0) {
    checkErrnoAndUpdateInterruptFlagOrExit();
  }
}

void UdpClient::checkErrnoAndUpdateInterruptFlagOrExit() {
  if (errno == EINTR) {
    hasPreviousOperationBeenInterruptedFlag = true;
  } else {
    exit(1);
  }
}

std::string UdpClient::readMessage(size_t size) {
  std::string buffer;
  buffer.resize(size);
  socklen_t rcvaLen = sizeof(latestClientAddress);

  hasPreviousOperationBeenInterruptedFlag = false;
  hasPreviousOperationTimeoutedFlag = false;

  if (recvfrom(socketId, &buffer[0], size, 0, (struct sockaddr *) &latestClientAddress, &rcvaLen) < 0) {
    checkErrnoAndUpdateInterruptFlagAndTimeoutFlagOrExit();
  }

  return buffer;
}

void UdpClient::checkErrnoAndUpdateInterruptFlagAndTimeoutFlagOrExit() {
  if (errno == EINTR) {
    hasPreviousOperationBeenInterruptedFlag = true;
  } else if (errno == EAGAIN) {
    hasPreviousOperationTimeoutedFlag = true;
  } else {
    exit(1);
  }
}


sockaddr_in UdpClient::getLatestClientAddress() {
  return latestClientAddress;
}


UdpClient::~UdpClient() {
  close(socketId);
}

bool UdpClient::hasPreviousOperationBeenInterrupted() {
  return hasPreviousOperationBeenInterruptedFlag;
}

bool UdpClient::hasPreviousOperationTimeouted() {
  return hasPreviousOperationTimeoutedFlag;
}

