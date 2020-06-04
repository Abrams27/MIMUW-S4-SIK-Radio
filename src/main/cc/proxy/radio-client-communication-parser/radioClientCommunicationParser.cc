#include "radioClientCommunicationParser.h"

#include <arpa/inet.h>

CommunicationType RadioClientCommunicationParser::parseHeader(std::string header) {
  uint16_t numberBeforeConversion = twoCharsToInt16(header[0], header[1]);
  uint16_t numberAfterConversion = ntohs(numberBeforeConversion);

  return getCommunicationTypeForNumber(numberAfterConversion);
}

uint16_t RadioClientCommunicationParser::twoCharsToInt16(char c1, char c2) {
  return ((uint16_t) c1) * (1 << 8) + (uint16_t) c2;
}

CommunicationType RadioClientCommunicationParser::getCommunicationTypeForNumber(
  uint16_t numberAfterConversion) {
  if (numberAfterConversion == DISCOVER) {
    return DISCOVER;
  } else if (numberAfterConversion == KEEPALIVE) {
    return KEEPALIVE;
  } else {
    return ERROR;
  }
}