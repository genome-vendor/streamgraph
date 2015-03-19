cmake_minimum_required(VERSION 2.8)

set(PROJ_LIBS ui process utility)

set(EXECUTABLE_NAME streamgraph)
add_executable(${EXECUTABLE_NAME} streamgraph.cpp)
target_link_libraries(${EXECUTABLE_NAME} ${PROJ_LIBS} ${Boost_LIBRARIES} ${GLOG_LIBRARIES})
set_target_properties(${EXECUTABLE_NAME} PROPERTIES PACKAGE_OUTPUT_NAME ${EXECUTABLE_NAME}${EXE_VERSION_SUFFIX})
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin/)

