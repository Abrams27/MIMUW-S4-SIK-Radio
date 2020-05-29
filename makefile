CC = g++-9
CFLAGS = -Wall -Wextra -O2 -std=c++14
LDFLAGS =

.PHONY: all tests clean


all: radio-proxy tests

radio-proxy: radioProxy.o audioStreamSinkFactory.o outputAudioStreamSink.o programUsagePrinter.o
	$(CC) $(LDFLAGS) -o $@ $^


radioProxy.o: src/main/cc/proxy/radioProxy.cc src/main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h src/main/cc/proxy/audio-stream-sinks/audioStreamSink.h src/main/cc/proxy/audio-stream-sinks/outputAudioStreamSink.h src/main/cc/utils/programUsagePrinter.h
	$(CC) $(CFLAGS) -c $<

audioStreamSinkFactory.o: src/main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.cc src/main/cc/proxy/audio-stream-sinks/audioStreamSinkFactory.h src/main/cc/proxy/audio-stream-sinks/audioStreamSink.h src/main/cc/proxy/audio-stream-sinks/outputAudioStreamSink.h
	$(CC) $(CFLAGS) -c $<

outputAudioStreamSink.o: src/main/cc/proxy/audio-stream-sinks/outputAudioStreamSink.cc src/main/cc/proxy/audio-stream-sinks/outputAudioStreamSink.h src/main/cc/proxy/audio-stream-sinks/audioStreamSink.h
	$(CC) $(CFLAGS) -c $<


programArgumentsParser.o: src/main/cc/utils/programArgumentsParser.cc src/main/cc/utils/programArgumentsParser.h
	$(CC) $(CFLAGS) -c $<

programUsagePrinter.o: src/main/cc/utils/programUsagePrinter.cc src/main/cc/utils/programUsagePrinter.h
	$(CC) $(CFLAGS) -c $<

tests: programArgumentsParserTest defaultRadioProxyArgumentsResolverTest

programArgumentsParserTest: programArgumentsParserTest.o programArgumentsParser.o
	$(CC) $(LDFLAGS) -o $@ $^

programArgumentsParserTest.o: src/test/cc/programArgumentsParserTest.cc src/main/cc/utils/programArgumentsParser.h
	$(CC) $(CFLAGS) -c $<


defaultRadioProxyArgumentsResolverTest: defaultRadioProxyArgumentsResolverTest.o programArgumentsParser.o
	$(CC) $(LDFLAGS) -o $@ $^

defaultRadioProxyArgumentsResolverTest.o: src/test/cc/defaultRadioProxyArgumentsResolverTest.cc
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o radio-proxy programArgumentsParserTest defaultRadioProxyArgumentsResolverTest