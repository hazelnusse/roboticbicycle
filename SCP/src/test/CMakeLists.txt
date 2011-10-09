add_executable(test_libserial test_libserial.cc)
target_link_libraries(test_libserial serial)

add_executable(test_termios test_termios.cc)
target_link_libraries(test_termios pthread)

add_executable(test_receiveserial test_receiveserial.cc ${PROJECT_SOURCE_DIR}/../MCP/src/sample.cc)
target_link_libraries(test_receiveserial pthread rt)
