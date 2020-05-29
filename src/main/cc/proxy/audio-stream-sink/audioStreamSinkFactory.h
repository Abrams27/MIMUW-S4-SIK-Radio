#ifndef MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H
#define MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H

#include "audioStreamSink.h"

#include <memory>

class AudioStreamSinkFactory {

public:
  static std::unique_ptr<AudioStreamSink> outputAudioStreamSink();
};

#endif //MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H
