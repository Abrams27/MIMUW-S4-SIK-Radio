#include <memory>

#include "../testUtils.h"
#include "../../../main/cc/proxy/response-parser/responseParser.h"
#include "../../../main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h"

static char *PROGRAM_NAME = "program";

const static std::string RESPONSE[] = {
  "123456778",
  "12321312312",
  "1231231243",
  "312312123"
};

int main() {
  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  for (const auto &responsePart : RESPONSE) {
    responseResolver->parseBody(responsePart);
  }

  return 0;
}
