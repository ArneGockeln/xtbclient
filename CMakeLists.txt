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
set(SOURCE_FILES Client.cpp Client.h StreamListener.cpp StreamListener.h RequestFactory.cpp RequestFactory.h Util.cpp Util.h)
set(SOURCE_FILES ${SOURCE_FILES} enums/Period.h enums/ApiType.h enums/MarginMode.h enums/ProfitMode.h enums/QuoteId.h enums/RequestStatus.h enums/Side.h enums/TransactionCmd.h enums/TransactionType.h enums/Weekdays.h)
set(SOURCE_FILES ${SOURCE_FILES} record/StreamBalanceRecord.h record/MarginLevelRecord.h record/NewsRecord.h record/QuoteRecord.h record/ServerTimeRecord.h record/StepRecord.h record/StepRuleRecord.h record/TradeTransactionStatusRecord.h record/TradingHourRecord.h record/TradeRecord.h record/TradingRecord.h record/TickRecord.h record/TradeTransactionInfo.h record/IBRecord.h record/UserDataRecord.h record/CommissionRecord.h record/ChartLastInfoRecord.h record/ChartRangeInfoRecord.h record/ChartLastRequestRecord.h record/CalendarRecord.h record/SymbolRecord.h record/ChartLastInfoRecord.h record/RateInfoRecord.h record/StreamCandleRecord.h record/StreamNewsRecord.h record/StreamProfitRecord.h record/StreamTickRecord.h record/StreamTradeRecord.h record/StreamTradeStatusRecord.h)
# compile shared library
add_library(xtbclient SHARED ${SOURCE_FILES})
# install
if("${CMAKE_BUILD_TYPE} equals Release")
    install(FILES ${SOURCE_FILES} DESTINATION include/xtbclient)
    install(TARGETS xtbclient EXPORT xtbclient-targets DESTINATION lib/xtbclient)
    install(EXPORT xtbclient-targets DESTINATION lib/xtbclient)
endif("${CMAKE_BUILD_TYPE} equals Release")