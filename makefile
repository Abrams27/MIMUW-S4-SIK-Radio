CC = g++-9
CFLAGS = -Wall -Wextra -O2
LDFLAGS =

.PHONY: all tests clean


all: radio-proxy tests

radio-proxy: radioProxy.o
	$(CC) $(LDFLAGS) -o $@ $^


radioProxy.o: src/main/cc/proxy/radioProxy.cc
	$(CC) $(CFLAGS) -c $<


programArgumentsParser.o: src/main/cc/utils/programArgumentsParser.cc src/main/cc/utils/programArgumentsParser.h
	$(CC) $(CFLAGS) -c $<


tests: programArgumentsParserTest

programArgumentsParserTest: programArgumentsParserTest.o programArgumentsParser.o
	$(CC) $(LDFLAGS) -o $@ $^

programArgumentsParserTest.o: src/test/cc/programArgumentsParserTest.cc src/main/cc/utils/programArgumentsParser.h
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f *.o radio-proxy programArgumentsParserTest