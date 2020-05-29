#ifndef MIMUW_S4_SIK_RADIO_DEFAULTRADIOPROXYARGUMENTSRESOLVER_H
#define MIMUW_S4_SIK_RADIO_DEFAULTRADIOPROXYARGUMENTSRESOLVER_H

#include "../../utils/programArgumentsParser.h"

#include <memory>

class DefaultRadioProxyArgumentsResolver {

public:
  DefaultRadioProxyArgumentsResolver(int argc, char *argv[]);


private:

  std::unique_ptr<ProgramArgumentsParser> programArgumentsParser;

  void parseRequiredArgumentsOrExit();
};


#endif //MIMUW_S4_SIK_RADIO_DEFAULTRADIOPROXYARGUMENTSRESOLVER_H
