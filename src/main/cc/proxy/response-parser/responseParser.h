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
  const std::string metadataHeaderRegexString = R"(icy-metaint:(\d*)\r\n)";
  const std::string headersEndString = "\r\n";

  std::unique_ptr<AudioStreamSink> audioStreamSink;
  std::unique_ptr<ProgramUsagePrinter> programUsagePrinter;
  bool metadataRequired;

  bool hasHeadersEndedFlag = false;
  bool hasMetadataDetected = false;

  int metadataInterval = 0;
  int bodyBytesRead = 0;

  int metadataBlockSize = 0;
  int metadataBytesRead = 0;
  bool hasMetadataSizeRead = false;

  bool isMetadataMode = false;

  bool isStatusLineAcceptable(const std::string &statusLine);
  bool isMetadataHeader(const std::string &header);
  int getMetadataInterval(const std::string &header);
  void resolveMetadataHeader(const std::string &header);
  void updateFlagIfHeadersEnded(const std::string &header);

  void parseAudioBlock(const std::string &line);
  void parseMetadataBlock(const std::string &line);

  bool willLineFitInInterval(const std::string &line);
  void updateMetadataModeIfIntervalEnded();

  void parseAudioBlockIfWillFitInInterval(const std::string &line);

  void parseAudioBlockIfWillNotFitInInterval(const std::string &line);

  void parseMetadataBlockSize(const std::string &line);

  void updateModeIfEndOfBlock();

  size_t getMetadataBlockSize(const std::string &line);

  void parseMetadataBlockIfLineWillFitInBlock(const std::string &line);

  void parseMetadataBlockIfLineWillNotFitInBlock(const std::string &line);

  void parseMetadataBlockIfSizeRead(const std::string &line);

  bool isParsingMetadataRequired() const;

  void parseBodyWithMetadata(const std::string &line);
};


#endif //MIMUW_S4_SIK_RADIO_RESPONSEPARSER_H
