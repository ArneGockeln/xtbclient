cmake_minimum_required(VERSION 3.8)
project(xtbclient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Users/arnegockeln/Sourcecode/cpp/includes -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto")

set(SOURCE_FILES main.cpp Client.cpp Client.h StreamListener.cpp StreamListener.h RequestFactory.cpp RequestFactory.h ResponseFactory.cpp ResponseFactory.h Util.cpp Util.h)
add_executable(xtbclient ${SOURCE_FILES})