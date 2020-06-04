#ifndef MIMUW_S4_SIK_RADIO_RADIOCLIENTCOMMUNICATIONPARSER_H
#define MIMUW_S4_SIK_RADIO_RADIOCLIENTCOMMUNICATIONPARSER_H

#include <string>

enum CommunicationType {
  DISCOVER = 1,
  IAM = 2,
  KEEPALIVE = 3,
  AUDIO = 4,
  METADATA = 6,
  ERROR = 0,
};

class RadioClientCommunicationParser {
public:

  static CommunicationType parseHeader(std::string header);

private:

  static uint16_t twoCharsToInt16(char c1, char c2) ;

  static CommunicationType
  getCommunicationTypeForNumber(uint16_t numberAfterConversion);
};


#endif //MIMUW_S4_SIK_RADIO_RADIOCLIENTCOMMUNICATIONPARSER_H
