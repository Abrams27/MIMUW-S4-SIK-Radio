#include "audio-stream-sink/audioStreamSinkFactory.h"

int main(int argc, char *argv[]) {

  AudioStreamSink* audioStreamSink = AudioStreamSinkFactory::outputAudioStreamSink();

  audioStreamSink->handleMetadata("METADATA");
  audioStreamSink->handleAudioData("AUDIO");

  delete audioStreamSink;

  return 0;
}