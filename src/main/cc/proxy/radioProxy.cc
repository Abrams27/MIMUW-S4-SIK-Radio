#include <memory>
#include <csignal>

#include "audio-stream-sinks/audioStreamSinkFactory.h"
#include "response-resolver/responseResolver.h"
#include "tcp-client/tcpClient.h"
#include "program-arguments-resolvers/defaultRadioProxyArgumentsResolver.h"
#include "program-arguments-resolvers/udpProxyArgumentsResolver.h"

#include <iostream>

bool run = true;

void intSignalHandler(int signal) {
  run = false;
}

void xd() {

}

int main(int argc, char *argv[]) {
  std::signal(SIGINT, intSignalHandler);

  std::unique_ptr<DefaultRadioProxyArgumentsResolver> defaultRadioProxyArgumentsResolver
    = std::make_unique<DefaultRadioProxyArgumentsResolver>(argc, argv);

  std::unique_ptr<UdpProxyArgumentsResolver> udpProxyArgumentsResolver
    = std::make_unique<UdpProxyArgumentsResolver>(argc, argv);

  std::unique_ptr<ResponseResolver> responseResolver
    = std::make_unique<ResponseResolver>(defaultRadioProxyArgumentsResolver->getMetadataOrDefault(), argv[0]);


  std::string host = defaultRadioProxyArgumentsResolver->getHost();
  std::string port = std::to_string(defaultRadioProxyArgumentsResolver->getPort());
  std::string resource = defaultRadioProxyArgumentsResolver->getResource();
  const int timeout = defaultRadioProxyArgumentsResolver->getTimeoutOrDefault();

  std::unique_ptr<TcpClient> tcpClient
    = std::make_unique<TcpClient>(host, port, resource, timeout);




  std::unique_ptr<AudioStreamSink> audioSink = AudioStreamSinkFactory::outputAudioStreamSink();

  tcpClient->sentRequest(defaultRadioProxyArgumentsResolver->getMetadataOrDefault());
  responseResolver->parseStatusLine(tcpClient->getResponseLine());

  while(responseResolver->hasHeadersEnded() == 0) {
    responseResolver->parseHeader(tcpClient->getResponseLine());
  }

  size_t interval = responseResolver->getAudioBlockSize();

  while(tcpClient->hasPreviousReadSucceed() && run) {
    std::string s = tcpClient->getResponseChunk(interval);
    if (tcpClient->hasPreviousReadSucceed()) {
      audioSink->handleAudioData(s);

      if (responseResolver->areMetadataParsing()) {
        std::string o = tcpClient->getResponseChunk(1);
        if (tcpClient->hasPreviousReadSucceed()) {
          size_t metSize = responseResolver->parseMetadataBlockSize(o);

          o = tcpClient->getResponseChunk(metSize);
          if (tcpClient->hasPreviousReadSucceed()) {
            audioSink->handleMetadata(o);
          }
        }
      }
    }
  }

  return 0;
}