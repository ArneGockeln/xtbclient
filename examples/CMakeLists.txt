cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)
set(L_DIR "/usr/local/opt")
# xtbclient library
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${L_DIR}/xtbclient -L/usr/local/lib -lxtbclient")
# date dependency
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${L_DIR}/date/include")

# executables
add_executable(symbol symbol.cpp)
add_executable(tickprices tickprices.cpp)
add_executable(crossing_sma_strategy crossing_sma_strategy.cpp)