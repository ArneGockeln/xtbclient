cmake_minimum_required(VERSION 3.8)
project(xtbclient)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Users/arnegockeln/Sourcecode/cpp/includes -I/usr/local/opt/openssl/include -L/usr/local/opt/openssl/lib -lssl -lcrypto")

set(SOURCE_FILES main.cpp Client.cpp Client.h StreamListener.cpp StreamListener.h RequestFactory.cpp RequestFactory.h Util.cpp Util.h)
set(SOURCE_FILES ${SOURCE_FILES} enums/Period.h enums/ApiType.h enums/MarginMode.h enums/ProfitMode.h enums/QuoteId.h enums/RequestStatus.h enums/Side.h enums/TransactionCmd.h enums/TransactionType.h enums/Weekdays.h)
set(SOURCE_FILES ${SOURCE_FILES} record/BalanceRecord.h record/MarginLevelRecord.h record/NewsRecord.h record/QuoteRecord.h record/ServerTimeRecord.h record/StepRecord.h record/StepRuleRecord.h record/TradeTransactionStatusRecord.h record/TradingHourRecord.h record/TradeRecord.h record/TradingRecord.h record/TickRecord.h record/TradeTransactionInfo.h record/IBRecord.h record/UserDataRecord.h record/CommissionRecord.h record/ChartLastInfoRecord.h record/ChartRangeInfoRecord.h record/ChartLastRequestRecord.h record/CalendarRecord.h record/SymbolRecord.h record/ChartLastInfoRecord.h record/RateInfoRecord.h main.cpp endpoints/ChartLastRequest.cpp endpoints/ChartLastRequest.h)
add_executable(xtbclient ${SOURCE_FILES})