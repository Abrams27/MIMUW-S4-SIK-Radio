#include <memory>

#include "../testUtils.h"
#include "../../../main/cc/proxy/response-parser/responseParser.h"
#include "../../../main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h"


static char *PROGRAM_NAME = "program";

const static std::string METAINT_HEADER = "icy-metaint:8\r\n";

const static std::string RESPONSE[] = {
  "12345", "678",
  "1",
  "12345678", "90",
  "123456",
  "12345678",
  "0",
  "2137"
};

int main() {
  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  responseResolver->parseHeader(METAINT_HEADER);

  for (const auto &responsePart : RESPONSE) {
    responseResolver->parseBody(responsePart);
  }

  return 0;
}
