#include "audioStreamSinkFactory.h"

#include "outputAudioStreamSink.h"
#include <memory>

std::unique_ptr<AudioStreamSink> AudioStreamSinkFactory::outputAudioStreamSink() {
  return std::make_unique<OutputAudioStreamSink>();
}
