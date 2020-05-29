#ifndef MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H
#define MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H

#include "audioStreamSink.h"

class AudioStreamSinkFactory {

public:
  static AudioStreamSink* outputAudioStreamSink();
};

#endif //MIMUW_S4_SIK_RADIO_AUDIOSTREAMSINKFACTORY_H
