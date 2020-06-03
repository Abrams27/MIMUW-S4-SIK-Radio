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
#include <memory>
#include <csignal>

#include "audio-stream-sinks/audioStreamSinkFactory.h"
#include "response-resolver/responseResolver.h"
#include "audio-stream-sinks/outputAudioStreamSink.h"
#include "tcp-client/tcpClient.h"

bool run = true;

void intSignalHandler(int signal) {
  run = false;
}


int main(int argc, char *argv[]) {
  std::signal(SIGINT, intSignalHandler);

  std::unique_ptr<ResponseResolver> responseResolver
    = std::make_unique<ResponseResolver>(true, argv[0]);
  std::unique_ptr<TcpClient> tcpClient = std::make_unique<TcpClient>("waw02-03.ic.smcdn.pl", "8000", "/t043-1.mp3");
  std::unique_ptr<AudioStreamSink> audioSink = AudioStreamSinkFactory::outputAudioStreamSink();

  tcpClient->sentRequest(true);

  responseResolver->parseStatusLine(tcpClient->getResponseLine());

  while(responseResolver->hasHeadersEnded() == 0) {
    responseResolver->parseHeader(tcpClient->getResponseLine());
  }

  size_t interval = responseResolver->getAudioBlockSize();

  while(tcpClient->hasPreviousReadSucceed() && run) {

    audioSink->handleAudioData(tcpClient->getResponseChunk(interval));

    if (responseResolver->areMetadataParsing()) {
      size_t metSize = responseResolver->parseMetadataBlockSize(tcpClient->getResponseChunk(1));

      audioSink->handleMetadata(tcpClient->getResponseChunk(metSize));
    }

  }

  return 0;
}