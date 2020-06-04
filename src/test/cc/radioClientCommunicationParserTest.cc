#include <cassert>
#include <memory>

#include "testUtils.h"
#include "../../main/cc/proxy/radio-client-communication-parser/radioClientCommunicationParser.h"

void shouldParseDiscoverHeader();
void shouldParseDiscoverKeepalive();
void shouldParseInvalidHeader();

char ZERO_CHAR = 0;
char ONE_CHAR = 1;
char THREE_CHAR = 3;

int main() {

  logTestFileName("radioClientCommunicationParserTest");

  shouldParseDiscoverHeader();
  shouldParseDiscoverKeepalive();
  shouldParseInvalidHeader();

  logAllTestsPassed();

  return 0;
}

void shouldParseDiscoverHeader() {
  logTest("should parse discover header");

  std::string header;
  header += ONE_CHAR;
  header += ZERO_CHAR;
  header += ZERO_CHAR;
  header += ZERO_CHAR;

  assert(RadioClientCommunicationParser::parseHeader(header) == DISCOVER);

  logPassedTest();
}

void shouldParseDiscoverKeepalive() {
  logTest("should parse keepalive header");

  std::string header;
  header += THREE_CHAR;
  header += ZERO_CHAR;
  header += ZERO_CHAR;
  header += ZERO_CHAR;

  assert(RadioClientCommunicationParser::parseHeader(header) == KEEPALIVE);

  logPassedTest();
}

void shouldParseInvalidHeader() {
  logTest("should parse invalid header");

  std::string header;
  header += THREE_CHAR;
  header += ONE_CHAR;
  header += ZERO_CHAR;
  header += ZERO_CHAR;

  assert(RadioClientCommunicationParser::parseHeader(header) == ERROR);

  logPassedTest();
}