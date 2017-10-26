cmake_minimum_required(VERSION 3.7)
project(xtbclient)
# Version
set(xtbclient_VERSION_MAJOR 1)
set(xtbclient_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)
# release build type
set(CMAKE_BUILD_TYPE Release)
# set inc directory
set(INC_DIR "/usr/local/include")

if(APPLE)
    # set ssl dir on macos
	set(SSL_DIR "/usr/local/opt/openssl@1.1")
elseif(UNIX)
    # set ssl dir on unix
	set(SSL_DIR "/usr/local/include/openssl")
endif(APPLE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${SSL_DIR}/include")
# date
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${INC_DIR}/date/include")
# rapidjson
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${INC_DIR}/rapidjson")
# source files
set(SOURCE_FILES src/Client.cpp src/Client.h src/StreamListener.cpp src/StreamListener.h src/RequestFactory.cpp src/RequestFactory.h src/Util.cpp src/Util.h)
set(SOURCE_FILES ${SOURCE_FILES} src/enums/Period.h src/enums/ApiType.h src/enums/MarginMode.h src/enums/ProfitMode.h src/enums/QuoteId.h src/enums/RequestStatus.h src/enums/Side.h src/enums/TransactionCmd.h src/enums/TransactionType.h src/enums/Weekdays.h)
set(SOURCE_FILES ${SOURCE_FILES} src/record/StreamBalanceRecord.h src/record/MarginLevelRecord.h src/record/NewsRecord.h src/record/QuoteRecord.h src/record/ServerTimeRecord.h src/record/StepRecord.h src/record/StepRuleRecord.h src/record/TradeTransactionStatusRecord.h src/record/TradingHourRecord.h src/record/TradeRecord.h src/record/TradingRecord.h src/record/TickRecord.h src/record/TradeTransactionInfo.h src/record/IBRecord.h src/record/UserDataRecord.h src/record/CommissionRecord.h src/record/ChartLastInfoRecord.h src/record/ChartRangeInfoRecord.h src/record/ChartLastRequestRecord.h src/record/CalendarRecord.h src/record/SymbolRecord.h src/record/ChartLastInfoRecord.h src/record/RateInfoRecord.h src/record/StreamCandleRecord.h src/record/StreamNewsRecord.h src/record/StreamProfitRecord.h src/record/StreamTickRecord.h src/record/StreamTradeRecord.h src/record/StreamTradeStatusRecord.h)

# compile shared library
add_library(xtbclient SHARED ${SOURCE_FILES})

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")

# linking
if(APPLE)
    target_link_libraries(xtbclient ${SSL_DIR}/lib/libssl.dylib)
    target_link_libraries(xtbclient ${SSL_DIR}/lib/libcrypto.dylib)
elseif(UNIX)
	target_link_libraries(xtbclient /usr/local/lib/libssl.so)
	target_link_libraries(xtbclient /usr/local/lib/libcrypto.so)
endif(APPLE)

# install header files to include/xtbclient
install(DIRECTORY src/ DESTINATION include/xtbclient CONFIGURATIONS Release FILES_MATCHING PATTERN "*.h")
# install library to lib/xtbclient
install(TARGETS xtbclient DESTINATION lib/xtbclient)

# build examples
#add_subdirectory(examples)
