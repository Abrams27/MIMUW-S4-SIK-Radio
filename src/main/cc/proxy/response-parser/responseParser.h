#ifndef MIMUW_S4_SIK_RADIO_RESPONSEPARSER_H
#define MIMUW_S4_SIK_RADIO_RESPONSEPARSER_H

#include "../audio-stream-sinks/audioStreamSink.h"
#include "../../utils/programUsagePrinter.h"

#include <memory>

class ResponseParser {

public:
  ResponseParser(std::unique_ptr<AudioStreamSink> audioStreamSink, bool metadataRequired, char *programName);

  void parseStatusLine(const std::string &statusLine);
  void parseHeader(const std::string &header);
  void parseBody(const std::string &line);
  bool hasHeadersEnded();

private:
  const static size_t possibleAcceptableStatusLinesSize = 3;
  std::string possibleAcceptableStatusLines[possibleAcceptableStatusLinesSize] = {
    "ICY 200 OK\r\n",
    "HTTP/1.0 200 OK\r\n",
    "HTTP/1.1 200 OK\r\n"
  };
  const std::string metadataHeaderRegexString = R"(icy-metaint:(\d*)\\r\\n)";
  const std::string headersEndString = "\r\n";

  std::unique_ptr<AudioStreamSink> audioStreamSink;
  std::unique_ptr<ProgramUsagePrinter> programUsagePrinter;
  bool metadataRequired;

  bool hasHeadersEndedFlag;
  int metadataInterval;

  bool isStatusLineAcceptable(const std::string &statusLine);
  bool isMetadataHeader(const std::string &header);
  int getMetadataInterval(const std::string &header);
  void resolveMetadataHeader(const std::string &header);
  void updateFlagIfHeadersEnded(const std::string &header);
};


#endif //MIMUW_S4_SIK_RADIO_RESPONSEPARSER_H
