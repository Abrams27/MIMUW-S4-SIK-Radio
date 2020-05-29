PROJECT(MIMUW-S4-SIK-Radio)

set(CMAKE_SOURCE_DIR src)
set (CMAKE_CXX_STANDARD 14)

# main
# proxy
add_executable(radio-proxy                              src/main/cc/proxy/radioProxy.cc)
add_executable(audioStreamSinkFactory                   src/main/cc/proxy/audio-stream-sink/audioStreamSinkFactory.cc)
add_executable(outputAudioStreamSink                    src/main/cc/proxy/audio-stream-sink/outputAudioStreamSink.cc)
# utils
add_executable(programArgumentsParser                   src/main/cc/utils/programArgumentsParser.cc)


# test
add_executable(programArgumentsParserTest               src/test/cc/programArgumentsParserTest.cc)