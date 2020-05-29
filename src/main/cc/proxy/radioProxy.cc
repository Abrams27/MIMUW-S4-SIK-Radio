#include "audio-stream-sink/audioStreamSinkFactory.h"

#include <memory>

int main(int argc, char *argv[]) {

  std::unique_ptr<AudioStreamSink> audioStreamSink = AudioStreamSinkFactory::outputAudioStreamSink();

  audioStreamSink->handleMetadata("METADATA");
  audioStreamSink->handleAudioData("AUDIO");

  return 0;
}