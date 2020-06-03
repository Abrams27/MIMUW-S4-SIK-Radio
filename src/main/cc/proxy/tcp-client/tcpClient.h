#ifndef MIMUW_S4_SIK_RADIO_TCPCLIENT_H
#define MIMUW_S4_SIK_RADIO_TCPCLIENT_H

#include <string>
#include <netdb.h>
#include <memory>
#include "../response-resolver/responseResolver.h"

class TcpClient {
public:
  TcpClient(std::string host, std::string port, std::string resource, int timeout = 5);

  virtual ~TcpClient();

  void sentRequest(bool metadataRequired);
  std::string getResponseLine();
  std::string getResponseChunk(size_t chunkSize);
  bool hasPreviousReadSucceed();

private:
  std::string host;
  std::string port;
  std::string resource;
  int timeout;

  int socketId;
  FILE *connectionFile;
  bool hasPreviousReadSucceedFlag = false;

  void initAddrResult(struct addrinfo *addr_result);
  addrinfo initAddrHints();
  void initSocket(struct addrinfo *addrResult);
  void startConnection(struct addrinfo *addrResult);
  void setTimeout();

  int sendRequestNoMetadata();
  int sentRequestMetadata();
};


#endif //MIMUW_S4_SIK_RADIO_TCPCLIENT_H
