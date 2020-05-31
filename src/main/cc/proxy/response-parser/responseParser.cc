#include "responseParser.h"
#include "../../utils/programUsagePrinter.h"

#include <memory>
#include <regex>

ResponseParser::ResponseParser(std::unique_ptr<AudioStreamSink> audioStreamSink, bool metadataRequired, char *programName) :
  programUsagePrinter(std::make_unique<ProgramUsagePrinter>(programName)),
  audioStreamSink(std::move(audioStreamSink)),
  metadataRequired(metadataRequired) {

  this->hasHeadersEndedFlag = false;
}

void ResponseParser::parseStatusLine(const std::string &statusLine) {
  if (!isStatusLineAcceptable(statusLine)) {
    programUsagePrinter->printUsageAndExitWith1();
  }
}

bool ResponseParser::isStatusLineAcceptable(const std::string &statusLine) {
  for (const auto &possibleAcceptableStatusLine : possibleAcceptableStatusLines) {
    if (statusLine == possibleAcceptableStatusLine) {
      return true;
    }
  }

  return false;
}


void ResponseParser::parseHeader(const std::string &header) {
  if (isMetadataHeader(header)) {
    resolveMetadataHeader(header);
  }

  updateFlagIfHeadersEnded(header);
}

bool ResponseParser::isMetadataHeader(const std::string &header) {
  std::regex metadataHeaderRegex(metadataHeaderRegexString);

  return std::regex_match(header, metadataHeaderRegex);
}

void ResponseParser::resolveMetadataHeader(const std::string &header) {
  if (metadataRequired) {
    metadataInterval = getMetadataInterval(header);
  } else {
    programUsagePrinter->printUsageAndExitWith1();
  }
}

int ResponseParser::getMetadataInterval(const std::string &header) {
  std::regex metadataHeaderRegex(metadataHeaderRegexString);
  std::smatch match;

  std::regex_search(header, match, metadataHeaderRegex);

  return std::stoi(match.str(1));
}

void ResponseParser::updateFlagIfHeadersEnded(const std::string &header) {
  if (header == headersEndString) {
    hasHeadersEndedFlag = true;
  }
}

void ResponseParser::parseBody(const std::string &line) {
  audioStreamSink->handleAudioData(line);
}

bool ResponseParser::hasHeadersEnded() {
  return hasHeadersEndedFlag;
}



