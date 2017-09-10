//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_REQUESTFACTORY_H
#define XTBCLIENT_REQUESTFACTORY_H

#include <string>
#include <rapidjson/document.h>
#include <rapidjson/pointer.h>
#include "enums/Period.h"
#include "record/ChartLastInfoRecord.h"
#include "record/ChartRangeInfoRecord.h"

namespace xtbclient {
  class RequestFactory {
  private:
    static std::string startCommand(std::string* t_command, std::string* t_streamSessionId);
    static std::string noArgumentCommand(std::string *t_command);
    static std::string getStringify(rapidjson::Document*);

  public:
    // single request commands
    static std::string getLogin(const char*, const char*, const char*, const char*);
    static std::string getAllSymbols();
    static std::string getCalendar();
    static std::string getCurrentUserData();
    static std::string getChartLastRequest(ChartLastInfoRecord &t_infoRecord);
    static std::string getChartRangeRequest(ChartRangeInfoRecord &t_infoRecord);
    static std::string getCommissionDef(const std::string& t_symbol, const double t_volume );

    // subscription commands
    static std::string subscribeBalance(std::string *t_streamSessionId);
    static std::string stopBalance();
    static std::string subscribeCandles(std::string* t_symbol, std::string* t_streamSessionId);
    static std::string stopCandles(std::string* t_symbol);
    static std::string subscribeKeepAlive(std::string* t_streamSessionId);
    static std::string stopKeepAlive();
    static std::string subscribeNews(std::string* t_streamSessionId);
    static std::string stopNews();
    static std::string subscribeProfits(std::string* t_streamSessionId);
    static std::string stopProfits();
    static std::string subscribeTickPrices(std::string* t_symbol, int t_minArrivalTime = 0, int t_maxLevel = -1, std::string* t_streamSessionId = nullptr);
    static std::string stopTickPrices(std::string* t_symbol);
    static std::string subscribeTrades(std::string* t_streamSessionId);
    static std::string stopTrades();
    static std::string subscribeTradeStatus(std::string* t_streamSessionId);
    static std::string stopTradeStatus();
  };
}


#endif //XTBCLIENT_REQUESTFACTORY_H
