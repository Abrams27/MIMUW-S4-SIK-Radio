#include "radioClientsConnectionWorker.h"

#include <utility>

radioClientsConnectionWorker::radioClientsConnectionWorker(std::shared_ptr<UdpClient> udpClient, std::shared_ptr<UdpClientsStorage> udpClientsStorage) :
  udpClient(std::move(udpClient)),
  udpClientsStorage(std::move(udpClientsStorage)),
  radioClientCommunicationParser(std::make_unique<RadioClientCommunicationParser>()) { }


void radioClientsConnectionWorker::work(const std::string &host, const std::string &port, const std::string &resource) {
  while(!interrupted) {
    workWhileNotInterrupted(host, port, resource);
  }
}

void radioClientsConnectionWorker::workWhileNotInterrupted(const std::string &host, const std::string &port, const std::string &resource) {
  std::string header = udpClient->readMessage(4);
  sockaddr_in client = udpClient->getLatestClientAddress();
  CommunicationType communicationType = radioClientCommunicationParser->parseHeader(header);

  resolveCommunicationType(host, port, resource, client, communicationType);
}

void radioClientsConnectionWorker::resolveCommunicationType(const std::string &host, const std::string &port, const std::string &resource, const sockaddr_in &client, const CommunicationType &communicationType) {
  if (communicationType == DISCOVER) {
    resolveCommunicationTypeDiscover(host, port, resource, client);
  } else if (communicationType == KEEPALIVE){
    resolveCommunicationTypeKeepalive();
  } else {
    exit(1);
  }
}

void radioClientsConnectionWorker::resolveCommunicationTypeDiscover(const std::string &host, const std::string &port, const std::string &resource, const sockaddr_in &client) {
  auto clientAddress = getClientAddress(client);
  std::string message = radioClientCommunicationParser->getMessageWithIam(host, port, resource);

  udpClientsStorage->addNewClient(clientAddress);
  udpClient->sendMessage(message, client);
}

void radioClientsConnectionWorker::resolveCommunicationTypeKeepalive(const sockaddr_in &client) {
  auto clientAddress = getClientAddress(client);

  udpClientsStorage->updateClientTimeoutAndRemoveIfExpired(clientAddress);
}

std::pair<uint16_t, uint32_t> radioClientsConnectionWorker::getClientAddress(sockaddr_in client) {
  uint16_t port = client.sin_port;
  uint32_t address = client.sin_addr.s_addr;

  return std::make_pair(port, address);
}


void radioClientsConnectionWorker::interrupt() {
  interrupted = true;
}
