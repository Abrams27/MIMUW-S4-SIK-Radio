#include "udpAudioStreamSink.h"

#include <utility>

UdpAudioStreamSink::UdpAudioStreamSink(std::shared_ptr<UdpClient> udpClient, std::shared_ptr<UdpClientsStorage> udpClientsStorage) :
  udpClient(std::move(udpClient)),
  udpClientsStorage(std::move(udpClientsStorage)),
  radioClientCommunicationParser(std::make_unique<RadioClientCommunicationParser>()) { }

#include <iostream>
void UdpAudioStreamSink::handleAudioData(std::string audioData) {
  auto messagesToSend = radioClientCommunicationParser->getMessageWithAudio(audioData);

  for (const auto &mesage : messagesToSend) {
    std::cout << "FOR -> " << audioData.size();
    sendMessageToClients(mesage);
  }
}


void UdpAudioStreamSink::handleMetadata(std::string metadata) {
  if (!metadata.empty()) {
    std::string messageToSend = radioClientCommunicationParser->getMessageWithMetadata(
      metadata);

    sendMessageToClients(messageToSend);
  }
}


void UdpAudioStreamSink::sendMessageToClients(const std::string &message) {
  auto clients = udpClientsStorage->getActiveClients();

  for (auto & client : clients) {
    sendMessageToClient(message, client);
  }
}


void UdpAudioStreamSink::sendMessageToClient(const std::string &message,const std::pair<uint16_t, uint32_t> &client) {
  std::cout << "SEND: ";
  std::cout << "port: " << client.first << "adress: " << client.second << std::endl;
  udpClient->sendMessage(message, client.first, client.second);
}
