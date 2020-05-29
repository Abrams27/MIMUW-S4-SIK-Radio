CC = g++-9
CFLAGS = -Wall -Wextra -O2 -std=c++14
LDFLAGS =

.PHONY: all tests clean


all: radio-proxy tests

radio-proxy: radioProxy.o audioStreamSinkFactory.o outputAudioStreamSink.o
	$(CC) $(LDFLAGS) -o $@ $^


radioProxy.o: src/main/cc/proxy/radioProxy.cc src/main/cc/proxy/audio-stream-sink/audioStreamSinkFactory.h src/main/cc/proxy/audio-stream-sink/audioStreamSink.h src/main/cc/proxy/audio-stream-sink/outputAudioStreamSink.h
	$(CC) $(CFLAGS) -c $<

audioStreamSinkFactory.o: src/main/cc/proxy/audio-stream-sink/audioStreamSinkFactory.cc src/main/cc/proxy/audio-stream-sink/audioStreamSinkFactory.h src/main/cc/proxy/audio-stream-sink/audioStreamSink.h src/main/cc/proxy/audio-stream-sink/outputAudioStreamSink.h
	$(CC) $(CFLAGS) -c $<

outputAudioStreamSink.o: src/main/cc/proxy/audio-stream-sink/outputAudioStreamSink.cc src/main/cc/proxy/audio-stream-sink/outputAudioStreamSink.h src/main/cc/proxy/audio-stream-sink/audioStreamSink.h
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