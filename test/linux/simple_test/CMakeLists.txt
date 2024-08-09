set(SOURCES simple_test.c asda.c sdo.c)
add_executable(simple_test ${SOURCES})
target_link_libraries(simple_test soem m)
install(TARGETS simple_test DESTINATION bin)
