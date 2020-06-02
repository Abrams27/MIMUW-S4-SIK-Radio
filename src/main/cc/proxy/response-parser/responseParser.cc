#include "responseParser.h"
#include "../../utils/programUsagePrinter.h"

#include <memory>
#include <regex>

ResponseParser::ResponseParser(std::unique_ptr<AudioStreamSink> audioStreamSink, bool metadataRequired, char *programName) :
  programUsagePrinter(std::make_unique<ProgramUsagePrinter>(programName)),
  audioStreamSink(std::move(audioStreamSink)),
  metadataRequired(metadataRequired){ }

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
    hasMetadataDetected = true;
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
  if (!line.empty()) {
    if (isParsingMetadataRequired()) {
      parseBodyWithMetadata(line);
    } else {
      parseAudioBlockIfWillFitInInterval(line);
    }
  }
}

void ResponseParser::parseBodyWithMetadata(const std::string &line) {
  if (isMetadataMode) {
    parseMetadataBlock(line);
  } else {
    parseAudioBlock(line);
  }
}

bool ResponseParser::isParsingMetadataRequired() {
  return hasMetadataDetected && metadataRequired;
}

bool ResponseParser::hasHeadersEnded() {
  return hasHeadersEndedFlag;
}

void ResponseParser::parseAudioBlock(const std::string &line) {
  if (willLineFitInInterval(line)) {
    parseAudioBlockIfWillFitInInterval(line);
  } else {
    parseAudioBlockIfWillNotFitInInterval(line);
  }
}

void ResponseParser::parseAudioBlockIfWillFitInInterval(const std::string &line) {
  bodyBytesRead += line.size();
  updateMetadataModeIfIntervalEnded();

  audioStreamSink->handleAudioData(line);
}

void ResponseParser::parseAudioBlockIfWillNotFitInInterval(const std::string &line) {
  const size_t spaceLeftForLine = metadataInterval - bodyBytesRead;
  bodyBytesRead = metadataInterval;
  updateMetadataModeIfIntervalEnded();

  audioStreamSink->handleAudioData(line.substr(0, spaceLeftForLine));

  parseBody(line.substr(spaceLeftForLine));
}

bool ResponseParser::willLineFitInInterval(const std::string &line) {
  return bodyBytesRead + line.size() <= metadataInterval;
}

void ResponseParser::updateMetadataModeIfIntervalEnded() {
  if (bodyBytesRead == metadataInterval) {
    isMetadataMode = true;
    bodyBytesRead = 0;
  }
}

void ResponseParser::parseMetadataBlock(const std::string &line) {
  if (hasMetadataSizeRead) {
    parseMetadataBlockIfSizeRead(line);
  } else {
    parseMetadataBlockSize(line);
    parseBody(line.substr(1));
  }
}

void ResponseParser::parseMetadataBlockIfSizeRead(const std::string &line) {
  if (metadataBytesRead + line.size() <= metadataBlockSize) {
    parseMetadataBlockIfLineWillFitInBlock(line);
  } else {
    parseMetadataBlockIfLineWillNotFitInBlock(line);
  }
}

void ResponseParser::parseMetadataBlockIfLineWillNotFitInBlock(const std::string &line) {
  const size_t spaceLeftForMetadata = metadataBlockSize - metadataBytesRead;
  metadataBytesRead = metadataBlockSize;
  updateModeIfEndOfBlock();

  audioStreamSink->handleMetadata(line.substr(0, spaceLeftForMetadata));

  parseBody(line.substr(spaceLeftForMetadata));
}

void ResponseParser::parseMetadataBlockIfLineWillFitInBlock(const std::string &line) {
  metadataBytesRead += line.size();
  updateModeIfEndOfBlock();

  audioStreamSink->handleMetadata(line);
}

void ResponseParser::parseMetadataBlockSize(const std::string &line) {
  metadataBlockSize = getMetadataBlockSize(line);
  metadataBytesRead = 0;
  hasMetadataSizeRead = true;

  updateModeIfEndOfBlock();
}

size_t ResponseParser::getMetadataBlockSize(const std::string &line) {
  size_t metadataSize = line[0];
  return metadataSize * metadataBlockMultiplicationFactor;
}

void ResponseParser::updateModeIfEndOfBlock() {
  if (metadataBytesRead == metadataBlockSize) {
    isMetadataMode = false;
    metadataBytesRead = 0;
    hasMetadataSizeRead = false;
  }
}



