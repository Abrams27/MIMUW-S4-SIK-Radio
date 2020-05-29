#ifndef MIMUW_S4_SIK_RADIO_OUTPUTAUDIOSTREAMSINK_H
#define MIMUW_S4_SIK_RADIO_OUTPUTAUDIOSTREAMSINK_H

#include "audioStreamSink.h"

class OutputAudioStreamSink : public AudioStreamSink {

public:
  OutputAudioStreamSink() = default;

  void handleAudioData(std::string audioData) override;
  void handleMetadata(std::string metadata) override;

  ~OutputAudioStreamSink() override = default;
};


#endif //MIMUW_S4_SIK_RADIO_OUTPUTAUDIOSTREAMSINK_H
