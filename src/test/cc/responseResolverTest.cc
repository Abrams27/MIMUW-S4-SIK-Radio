#include <iostream>
#include <cassert>
#include <memory>

#include "testUtils.h"
#include "../../main/cc/utils/programArgumentsParser.h"
#include "../../main/cc/proxy/response-parser/responseParser.h"
#include "../../main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h"

void shouldParseHIcyOK();
void shouldParseHttp10OK();
void shouldParseHttp11OK();
void shouldParseHeaders();

static char *PROGRAM_NAME = "program";

const static std::string VALID_STATUS_LINE_1 = "ICY 200 OK\r\n";
const static std::string VALID_STATUS_LINE_2 = "HTTP/1.0 200 OK\r\n";
const static std::string VALID_STATUS_LINE_3 = "HTTP/1.1 200 OK\r\n";

const static std::string HEADERS_TEST[] = {
  "icy-notice1:<BR>This stream requires <a href=\"http://www.winamp.com/\">Winamp</a><BR>\r\n",
  "icy-notice2:SHOUTcast Distributed Network Audio Server/FreeBSD v1.9.5<BR>\r\n",
  "icy-name:Orban Opticodec-PC\r\n",
  "icy-genre:Various\r\n",
  "icy-url:http://www.domain.com\r\n",
  "content-type:audio/aacp\r\n",
  "icy-pub:1\r\n",
  "icy-metaint:8192\r\n",
  "icy-br:32\r\n",
  "\r\n"
};

int main() {

  logTestFileName("responseResolverTest");

  shouldParseHIcyOK();
  shouldParseHttp10OK();
  shouldParseHttp11OK();
  shouldParseHeaders();

  logAllTestsPassed();

  return 0;
}

void shouldParseHIcyOK() {
  logTest("should parse icy 200 OK");

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  responseResolver->parseStatusLine(VALID_STATUS_LINE_1);

  logPassedTest();
}

void shouldParseHttp10OK() {
  logTest("should parse HTTP/1.0 200 OK");

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  responseResolver->parseStatusLine(VALID_STATUS_LINE_2);

  logPassedTest();
}

void shouldParseHttp11OK() {
  logTest("should parse HTTP/1.1 200 OK");

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  responseResolver->parseStatusLine(VALID_STATUS_LINE_3);

  logPassedTest();
}

void shouldParseHeaders() {
  logTest("should parse headers");

  std::unique_ptr<ResponseParser> responseResolver
    = std::make_unique<ResponseParser>(AudioStreamSinkFactory::outputAudioStreamSink(), true, PROGRAM_NAME);

  for (const auto & i : HEADERS_TEST) {
    responseResolver->parseHeader(i);
  }

  assert(responseResolver->hasHeadersEnded());

  logPassedTest();
}