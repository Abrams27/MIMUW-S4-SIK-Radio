#include "defaultRadioProxyArgumentsResolver.h"

DefaultRadioProxyArgumentsResolver::DefaultRadioProxyArgumentsResolver(int argc, char **argv) {
  this->programArgumentsParser = std::make_unique<ProgramArgumentsParser>(argc, argv);

  parseRequiredArgumentsOrExit()
}

void DefaultRadioProxyArgumentsResolver::parseRequiredArgumentsOrExit() {

}
