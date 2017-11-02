/*!
 *  Copyright(c)2017, Arne Gockeln
 *  http://www.arnegockeln.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XTBCLIENT_REQUESTFACTORY_H
#define XTBCLIENT_REQUESTFACTORY_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include <vector>
#include "enums/Period.h"
#include "enums/TransactionCmd.h"
#include "record/ChartLastInfoRecord.h"
#include "record/ChartRangeInfoRecord.h"
#include "record/TradeTransactionInfo.h"

namespace xtbclient {
  class RequestFactory {
  private:
    static std::string startCommand(const char* t_command, const char* t_streamSessionId);
    static std::string noArgumentCommand(const char* t_command);
    static std::string getStringify(rapidjson::Document*);

  public:
    // single request commands
    static std::string getLogin(const char*, const char*, const char*, const char*);
    static std::string getAllSymbols();
    static std::string getCalendar();
    static std::string getCurrentUserData();
    static std::string getChartLastRequest(ChartLastInfoRecord &t_infoRecord);
    static std::string getChartRangeRequest(ChartRangeInfoRecord &t_infoRecord);
    static std::string getCommissionDef(const std::string& t_symbol, double t_volume );
    static std::string getIbsHistory(long long t_start, long long t_end);
    static std::string getMarginLevel();
    static std::string getMarginTrade(const std::string& t_symbol, double t_volume );
    static std::string getNews(long long t_start, long long t_end);
    static std::string getProfitCalculation(double t_closePrice, double t_openPrice, TransactionCmd t_cmd, const std::string& t_symbol, double t_volume);
    static std::string getServerTime();
    static std::string getStepRules();
    static std::string getSymbol(const std::string& t_symbol);
    static std::string getTickPrices(int t_level, long long t_timestamp, std::vector<std::string> t_symbols);
    static std::string getTradeRecords(std::vector<unsigned long int> t_orders);
    static std::string getTrades(bool t_openedOnly);
    static std::string getTradesHistory(long long t_start, long long t_end);
    static std::string getTradingHours(std::vector<std::string> t_symbols);
    static std::string getVersion();
    static std::string getPing();
    static std::string getTradeTransaction(TradeTransactionInfo& t_info);
    static std::string getTradeTransactionStatus(unsigned long int t_order);

    // subscription commands
    static std::string subscribeBalance(const char* t_streamSessionId);
    static std::string stopBalance();
    static std::string subscribeCandles(std::string* t_symbol, const char* t_streamSessionId);
    static std::string stopCandles(std::string* t_symbol);
    static std::string subscribeKeepAlive(const char* t_streamSessionId);
    static std::string stopKeepAlive();
    static std::string subscribeNews(const char* t_streamSessionId);
    static std::string stopNews();
    static std::string subscribeProfits(const char* t_streamSessionId);
    static std::string stopProfits();
    static std::string subscribeTickPrices(std::string* t_symbol, int t_minArrivalTime = 0, int t_maxLevel = -1, const char* t_streamSessionId = nullptr);
    static std::string stopTickPrices(std::string* t_symbol);
    static std::string subscribeTrades(const char* t_streamSessionId);
    static std::string stopTrades();
    static std::string subscribeTradeStatus(const char* t_streamSessionId);
    static std::string stopTradeStatus();
  };
}


#endif //XTBCLIENT_REQUESTFACTORY_H
