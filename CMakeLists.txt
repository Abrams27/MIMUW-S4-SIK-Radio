PROJECT(MIMUW-S4-SIK-Radio)

set(CMAKE_SOURCE_DIR src)


# main
add_executable(radio-proxy                              src/main/cc/proxy/radioProxy.cc)
add_executable(programArgumentsParser                   src/main/cc/utils/programArgumentsParser.cc)


# test
add_executable(programArgumentsParserTest               src/test/cc/programArgumentsParserTest.cc)