cmake_minimum_required(VERSION 3.8)
project(xtbclient)
# Version
set(xtbclient_VERSION_MAJOR 1)
set(xtbclient_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)
# release build type
set(CMAKE_BUILD_TYPE Release)
# set opt directory
set(L_DIR "/usr/local/opt")
# openssl
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${L_DIR}/openssl/include -L${L_DIR}/openssl/lib -lssl -lcrypto")
# date
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${L_DIR}/date/include")
# rapidjson
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${L_DIR}/rapidjson")
# source files
set(SOURCE_FILES src/Client.cpp src/Client.h src/StreamListener.cpp src/StreamListener.h src/RequestFactory.cpp src/RequestFactory.h src/Util.cpp src/Util.h)
set(SOURCE_FILES ${SOURCE_FILES} src/enums/Period.h src/enums/ApiType.h src/enums/MarginMode.h src/enums/ProfitMode.h src/enums/QuoteId.h src/enums/RequestStatus.h src/enums/Side.h src/enums/TransactionCmd.h src/enums/TransactionType.h src/enums/Weekdays.h)
set(SOURCE_FILES ${SOURCE_FILES} src/record/StreamBalanceRecord.h src/record/MarginLevelRecord.h src/record/NewsRecord.h src/record/QuoteRecord.h src/record/ServerTimeRecord.h src/record/StepRecord.h src/record/StepRuleRecord.h src/record/TradeTransactionStatusRecord.h src/record/TradingHourRecord.h src/record/TradeRecord.h src/record/TradingRecord.h src/record/TickRecord.h src/record/TradeTransactionInfo.h src/record/IBRecord.h src/record/UserDataRecord.h src/record/CommissionRecord.h src/record/ChartLastInfoRecord.h src/record/ChartRangeInfoRecord.h src/record/ChartLastRequestRecord.h src/record/CalendarRecord.h src/record/SymbolRecord.h src/record/ChartLastInfoRecord.h src/record/RateInfoRecord.h src/record/StreamCandleRecord.h src/record/StreamNewsRecord.h src/record/StreamProfitRecord.h src/record/StreamTickRecord.h src/record/StreamTradeRecord.h src/record/StreamTradeStatusRecord.h)

# compile shared library
add_library(xtbclient SHARED ${SOURCE_FILES})

# install header files to include/xtbclient
install(DIRECTORY src/ DESTINATION include/xtbclient CONFIGURATIONS Release FILES_MATCHING PATTERN "*.h")
# install library to lib/xtbclient
install(TARGETS xtbclient DESTINATION lib/xtbclient)

# build examples
add_subdirectory(examples)
