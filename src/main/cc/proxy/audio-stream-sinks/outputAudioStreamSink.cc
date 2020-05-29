#include <iostream>

#include "outputAudioStreamSink.h"

void OutputAudioStreamSink::handleAudioData(std::string audioData) {
  std::cout << audioData;
}

void OutputAudioStreamSink::handleMetadata(std::string metadata) {
  std::cerr << metadata;
}
