cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(INC_DIR "/usr/local/include")
set(OPT_DIR "/usr/local/opt")
set(LIB_DIR "/usr/local/lib")
# xtbclient library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${INC_DIR}/xtbclient")
# date dependency
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${INC_DIR}/date/include")

# executables
add_executable(symbol symbol.cpp)
add_executable(tickprices tickprices.cpp)
#add_executable(crossing_sma_strategy crossing_sma_strategy.cpp)

target_link_libraries(symbol ${LIB_DIR}/xtbclient/libxtbclient.dylib)
target_link_libraries(tickprices ${LIB_DIR}/xtbclient/libxtbclient.dylib)
#target_link_libraries(crossing_sma_strategy ${LIB_DIR}/xtbclient/libxtbclient.dylib)