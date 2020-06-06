#include "radioProxyWorker.h"

RadioProxyWorker::RadioProxyWorker(std::shared_ptr<TcpClient> tcpClient, std::shared_ptr<ResponseResolver> responseResolver, std::shared_ptr<AudioStreamSink> audioStreamSink) :
  tcpClient(std::move(tcpClient)),
  responseResolver(std::move(responseResolver)),
  audioStreamSink(std::move(audioStreamSink)) {}

void RadioProxyWorker::work(bool metadataRead) {
  tcpClient->sentRequest(metadataRead);
  responseResolver->parseStatusLine(tcpClient->getResponseLine());

  readAndParseHeaders();
  const size_t metadataInterval = responseResolver->getAudioBlockSize();

  //TODO
  while(tcpClient->hasPreviousReadSucceed()) {
    readAndParseData(metadataInterval);
  }
}

void RadioProxyWorker::readAndParseHeaders() const {
  while(responseResolver->hasHeadersEnded() == 0) {
    responseResolver->parseHeader(tcpClient->getResponseLine());
  }
}

void RadioProxyWorker::readAndParseData(const size_t metadataInterval) const {
  std::string dataChunk = tcpClient->getResponseChunk(metadataInterval);

  if (tcpClient->hasPreviousReadSucceed()) {
    handleDataAndReadMetadataIfRequired(dataChunk);
  }
}

void RadioProxyWorker::handleDataAndReadMetadataIfRequired(const std::string &dataChunk) const {
  audioStreamSink->handleAudioData(dataChunk);

  if (responseResolver->areMetadataParsing()) {
    readAndParseMetadata();
  }
}

void RadioProxyWorker::readAndParseMetadata() const {
  std::string metadataSizeChunk = tcpClient->getResponseChunk(1);

  if (tcpClient->hasPreviousReadSucceed()) {
    handleMetadataSizeAndReadMetadata(metadataSizeChunk);
  }
}

void RadioProxyWorker::handleMetadataSizeAndReadMetadata(std::string &metadataSizeChunk) const {
  size_t metadataSize = responseResolver->parseMetadataBlockSize(metadataSizeChunk);
  std::string metadataChunk = tcpClient->getResponseChunk(metadataSize);

  if (tcpClient->hasPreviousReadSucceed()) {
    audioStreamSink->handleMetadata(metadataSizeChunk);
  }
}

void RadioProxyWorker::interrupt() {

}
