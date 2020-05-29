#include "audioStreamSinkFactory.h"

#include "outputAudioStreamSink.h"

AudioStreamSink *AudioStreamSinkFactory::outputAudioStreamSink() {
  return new OutputAudioStreamSink();
}
