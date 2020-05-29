#include <iostream>
#include <cassert>

#include "testUtils.h"
#include "../../main/cc/proxy/program-arguments-resolvers/defaultRadioProxyArgumentsResolver.h"


//-h host – nazwa serwera udostępniającego strumień audio, obowiązkowy;
//-r resource – nazwa zasobu na serwerze, zwykle sam ukośnik, obowiązkowy;
//-p port – numer portu, na którym serwera udostępniającego strumień audio, liczba dziesiętna, obowiązkowy;
//-m yes|no – wartość yes, jeśli program powinien żądać od serwera przesyłania metadanych, domyślnie wartość no oznaczająca, że program nie powinien żądać od serwera metadanych, opcjonalny;
//-t timeout – czas w sekundach, po którego upłynięciu, jeśli serwer nic nie przysłał, pośrednik uznaje, że serwer przestał działać, domyślna wartość 5 sekund, opcjonalny.

int main() {

  logTestFileName("defaultRadioProxyArgumentsResolverTest");


  logAllTestsPassed();

  return 0;
}