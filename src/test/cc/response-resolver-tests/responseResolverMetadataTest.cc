#include <memory>

#include "../testUtils.h"
#include "../../../main/cc/proxy/response-parser/responseParser.h"
#include "../../../main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h"


static char *PROGRAM_NAME = "program";

const static std::string METAINT_HEADER = "icy-metaint:8\r\n";

int main() {
  std::string zeroString;
  zeroString = (char) 0;

  std::string oneString;
  oneString = (char) 1;

  std::string RESPONSE[] = {
    "12345", "678" +
    oneString +
    "12345678", "90",
    "12345612345678" +
    zeroString +
    "12345678",
    zeroString +
    "12345678",
    zeroString +
    "12345678" +
    oneString +
    "12345678", "90",
    "123456",
  };

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  responseResolver->parseHeader(METAINT_HEADER);

  for (const auto &responsePart : RESPONSE) {
    responseResolver->parseBody(responsePart);
  }

  return 0;
}
