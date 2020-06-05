#include "audioStreamSinkFactory.h"

#include "outputAudioStreamSink.h"
#include "udpAudioStreamSink.h"
#include <memory>

std::unique_ptr<AudioStreamSink> AudioStreamSinkFactory::outputAudioStreamSink() {
  return std::make_unique<OutputAudioStreamSink>();
}

std::unique_ptr<AudioStreamSink> AudioStreamSinkFactory::udpAudioStreamSink(std::shared_ptr<UdpClient> udpClient, std::shared_ptr<UdpClientsStorage> udpClientsStorage) {
  return std::make_unique<UdpAudioStreamSink>(udpClient, udpClientsStorage);
}
