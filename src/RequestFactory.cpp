//
// Created by Arne Gockeln on 02.09.17.
//

#include "RequestFactory.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <sstream>

namespace xtbclient {
  using namespace rapidjson;

  /*!
   * Get login request
   *
   * @param const char* t_userId
   * @param const char* t_password
   * @param const char* t_appId
   * @param const char* t_appName
   * @return std::string
   */
  std::string RequestFactory::getLogin(const char* t_userId, const char* t_password, const char* t_appId = nullptr, const char* t_appName = nullptr) {
    Document document;

    SetValueByPointer(document, "/command", "login");
    SetValueByPointer(document, "/arguments/userId", StringRef(t_userId));
    SetValueByPointer(document, "/arguments/password", StringRef(t_password));
    if(t_appId != nullptr){
      SetValueByPointer(document, "/arguments/appId", StringRef(t_appId));
    }
    if(t_appName != nullptr){
      SetValueByPointer(document, "/arguments/appName", StringRef(t_appName));
    }

    return getStringify(&document);
  }

  /*!
   * Get all symbols request
   *
   * @return std::string
   */
  std::string RequestFactory::getAllSymbols() {
    return noArgumentCommand("getAllSymbols");
  }

  /*!
   * Get calendar request
   *
   * @return std::string
   */
  std::string RequestFactory::getCalendar() {
    return noArgumentCommand("getCalendar");
  }

  /*!
   * Get current user data request
   *
   * @return std::string
   */
  std::string RequestFactory::getCurrentUserData() {
    return noArgumentCommand("getCurrentUserData");
  }

  /**
   * Get chart info, from start date to the current time
   *
   * @param ChartLastInfoRecord
   * @return std::string
   */
  std::string RequestFactory::getChartLastRequest(ChartLastInfoRecord &t_infoRecord){
    Document document;

    SetValueByPointer(document, "/command", "getChartLastRequest");
    SetValueByPointer(document, "/arguments/info/period", static_cast<int>(t_infoRecord.m_period));
    SetValueByPointer(document, "/arguments/info/start", static_cast<int64_t>(t_infoRecord.m_start));
    SetValueByPointer(document, "/arguments/info/symbol", StringRef(t_infoRecord.m_symbol.c_str()));

    return getStringify(&document);
  }

  /*!
   * Get chart range info, from start to end with period and symbol
   *
   * @param ChartRangeInfoRecord t_infoRecord
   * @return std::string
   */
  std::string RequestFactory::getChartRangeRequest(ChartRangeInfoRecord &t_infoRecord) {
    Document document;

    SetValueByPointer(document, "/command", "getChartRangeRequest");
    SetValueByPointer(document, "/arguments/info/end", static_cast<int64_t>(t_infoRecord.m_end));
    SetValueByPointer(document, "/arguments/info/period", static_cast<int>(t_infoRecord.m_period));
    SetValueByPointer(document, "/arguments/info/start", static_cast<int64_t>(t_infoRecord.m_start));
    SetValueByPointer(document, "/arguments/info/symbol", StringRef(t_infoRecord.m_symbol.c_str()));
    SetValueByPointer(document, "/arguments/info/ticks", t_infoRecord.m_ticks);

    return getStringify(&document);
  }

  /*!
   * Get commision def request
   *
   * @param std::string& t_symbol
   * @param double t_volume
   * @return std::string
   */
  std::string RequestFactory::getCommissionDef(const std::string &t_symbol, const double t_volume) {
    Document document;

    SetValueByPointer(document, "/command", "getCommissionDef");
    SetValueByPointer(document, "/arguments/symbol", StringRef(t_symbol.c_str()));
    SetValueByPointer(document, "/arguments/volume", t_volume);

    return getStringify(&document);
  }

  /*!
   * Get IBs history data request
   *
   * @param const long long t_start
   * @param const long long t_end
   * @return std::string
   */
  std::string RequestFactory::getIbsHistory(const long long t_start, const long long t_end) {
    Document document;

    SetValueByPointer(document, "/command", "getIbsHistory");
    SetValueByPointer(document, "/arguments/start", static_cast<int64_t>(t_start));
    SetValueByPointer(document, "/arguments/end", static_cast<int64_t>(t_end));

    return getStringify(&document);
  }

  /*!
   * Get margin level request
   *
   * @return std::string
   */
  std::string RequestFactory::getMarginLevel() {
    return noArgumentCommand("getMarginLevel");
  }

  /*!
   * Get margin trade request
   *
   * @param const std::string& t_symbol
   * @param double t_volume
   * @return std::string
   */
  std::string RequestFactory::getMarginTrade(const std::string &t_symbol, double t_volume) {
    Document document;

    SetValueByPointer(document, "/command", "getMarginTrade");
    SetValueByPointer(document, "/arguments/symbol", StringRef(t_symbol.c_str()));
    SetValueByPointer(document, "/arguments/volume", t_volume);

    return getStringify(&document);
  }

  /*!
   * Get news request
   *
   * @param long long t_start
   * @param long long t_end
   * @return std::string
   */
  std::string RequestFactory::getNews(long long t_start, long long t_end) {
    Document document;

    SetValueByPointer(document, "/command", "getNews");
    SetValueByPointer(document, "/arguments/start", static_cast<int64_t>(t_start));
    SetValueByPointer(document, "/arguments/end", static_cast<int64_t>(t_end));

    return getStringify(&document);
  }

  /*!
   * Get profit calculation request
   *
   * @param double t_closePrice
   * @param double t_openPrice
   * @param TransactionCmd t_cmd
   * @param std::string& t_symbol
   * @param double t_volume
   * @return std::string
   */
  std::string RequestFactory::getProfitCalculation(double t_closePrice, double t_openPrice, TransactionCmd t_cmd,
                                                   const std::string &t_symbol, double t_volume) {
    Document document;

    SetValueByPointer(document, "/command", "getProfitCalculation");
    SetValueByPointer(document, "/arguments/closePrice", t_closePrice);
    SetValueByPointer(document, "/arguments/cmd", static_cast<int>(t_cmd));
    SetValueByPointer(document, "/arguments/openPrice", t_openPrice);
    SetValueByPointer(document, "/arguments/symbol", t_symbol.c_str());
    SetValueByPointer(document, "/arguments/volume", t_volume);

    return getStringify(&document);
  }

  /*!
   * Get server time request
   *
   * @return std::string
   */
  std::string RequestFactory::getServerTime() {
    return noArgumentCommand("getServerTime");
  }

  /*!
   * Get step rules request
   *
   * @return std::string
   */
  std::string RequestFactory::getStepRules() {
    return noArgumentCommand("getStepRules");
  }

  /*!
   * Get symbol request
   *
   * @param const std::string& t_symbol
   * @return std::string
   */
  std::string RequestFactory::getSymbol(const std::string &t_symbol) {
    Document document;

    SetValueByPointer(document, "/command", "getSymbol");
    SetValueByPointer(document, "/arguments/symbol", t_symbol.c_str());

    return getStringify(&document);
  }

  /*!
   * Get tick prices request, alternative is the streaming command!
   *
   * @param int t_level
   * @param long long t_timestamp
   * @param std::vector<std::string> t_symbols
   * @return std::string
   */
  std::string RequestFactory::getTickPrices(int t_level, long long t_timestamp, std::vector<std::string> t_symbols) {
    Document document;

    SetValueByPointer(document, "/command", "getTickPrices");
    SetValueByPointer(document, "/arguments/level", t_level);
    SetValueByPointer(document, "/arguments/timestamp", static_cast<int64_t>(t_timestamp));

    Value symbols(kArrayType);
    Document::AllocatorType& allocator = document.GetAllocator();

    for(std::string &s : t_symbols){
      symbols.PushBack(StringRef(s.c_str()), allocator);
    }

    SetValueByPointer(document, "/arguments/symbols", symbols);

    return getStringify(&document);
  }

  /*!
   * Get trade records request
   *
   * @param std::vector<unsigned long int> t_orders
   * @return std::string
   */
  std::string RequestFactory::getTradeRecords(std::vector<unsigned long int> t_orders) {
    Document document;

    Document::AllocatorType& allocator = document.GetAllocator();

    SetValueByPointer(document, "/command", "getTradeRecords");

    Value orders(kArrayType);
    for(auto &order : t_orders){
      // convert unsigned long int to string
      std::ostringstream ostr;
      ostr << order;

      // set json value
      Value v;
      v.SetString(StringRef(ostr.str().c_str()));
      orders.PushBack(v, allocator);
    }

    SetValueByPointer(document, "/arguments/orders", orders);

    return getStringify(&document);
  }

  /*!
   * Get trades request
   *
   * @param bool t_openedOnly
   * @return std::string
   */
  std::string RequestFactory::getTrades(bool t_openedOnly) {
    Document document;

    SetValueByPointer(document, "/command", "getTrades");
    SetValueByPointer(document, "/arguments/openedOnly", t_openedOnly);

    return getStringify(&document);
  }

  /*!
   * Get trades history request
   *
   * @param long long t_start
   * @param long long t_end
   * @return std::string
   */
  std::string RequestFactory::getTradesHistory(long long t_start, long long t_end) {
    Document document;

    SetValueByPointer(document, "/command", "getTradesHistory");
    SetValueByPointer(document, "/arguments/start", static_cast<int64_t>(t_start));
    SetValueByPointer(document, "/arguments/end", static_cast<int64_t>(t_end));

    return getStringify(&document);
  }

  /*!
   * Get trading hours request
   *
   * @param std::vector<std::string> t_symbols
   * @return std::string
   */
  std::string RequestFactory::getTradingHours(std::vector<std::string> t_symbols) {
    Document document;

    SetValueByPointer(document, "/command", "getTradingHours");

    Value symbols(kArrayType);
    Document::AllocatorType& allocator = document.GetAllocator();

    for(auto &sym : t_symbols){
      symbols.PushBack(StringRef(sym.c_str()), allocator);
    }

    SetValueByPointer(document, "/arguments/symbols", symbols);

    return getStringify(&document);
  }

  /*!
   * Get version request
   *
   * @return std::string
   */
  std::string RequestFactory::getVersion() {
    return noArgumentCommand("getVersion");
  }

  /*!
   * Get ping request
   *
   * @return std::string
   */
  std::string RequestFactory::getPing() {
    return noArgumentCommand("ping");
  }

  /*!
   * Get trade transaction request
   *
   * @param TradeTransactionInfo& t_info
   * @return std::string
   */
  std::string RequestFactory::getTradeTransaction(TradeTransactionInfo &m_tradeTransInfo) {
    Document document;

    SetValueByPointer(document, "/command", "tradeTransaction");
    SetValueByPointer(document, "/arguments/tradeTransInfo/cmd", m_tradeTransInfo.m_cmd);
    SetValueByPointer(document, "/arguments/tradeTransInfo/customComment", StringRef(m_tradeTransInfo.m_customComment.c_str()));
    SetValueByPointer(document, "/arguments/tradeTransInfo/expiration", m_tradeTransInfo.m_expiration);
    SetValueByPointer(document, "/arguments/tradeTransInfo/offset", m_tradeTransInfo.m_offset);

    // convert unsigned long int to uint64_t
    uint64_t order64 = static_cast<uint64_t>(m_tradeTransInfo.m_order);

    SetValueByPointer(document, "/arguments/tradeTransInfo/order", order64);
    SetValueByPointer(document, "/arguments/tradeTransInfo/price", m_tradeTransInfo.m_price);
    SetValueByPointer(document, "/arguments/tradeTransInfo/sl", m_tradeTransInfo.m_sl);
    SetValueByPointer(document, "/arguments/tradeTransInfo/symbol", m_tradeTransInfo.m_symbol.c_str());
    SetValueByPointer(document, "/arguments/tradeTransInfo/tp", m_tradeTransInfo.m_tp);
    SetValueByPointer(document, "/arguments/tradeTransInfo/type", m_tradeTransInfo.m_type);
    SetValueByPointer(document, "/arguments/tradeTransInfo/volume", m_tradeTransInfo.m_volume);

    return getStringify(&document);
  }

  /*!
   * Get trade transaction status request
   *
   * @param unsigned long int t_order
   * @return std::string
   */
  std::string RequestFactory::getTradeTransactionStatus(unsigned long int t_order) {
    Document document;

    // convert unsigned long int to uint64_t
    uint64_t order64 = static_cast<uint64_t>(t_order);

    SetValueByPointer(document, "/command", "tradeTransactionStatus");
    SetValueByPointer(document, "/arguments/order", order64);

    return getStringify(&document);
  }

  /*!
   * Get request for balance subscription
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeBalance(const char* t_streamSessionId) {
    return startCommand("getBalance", t_streamSessionId);
  }

  /*!
   * Get request to stop balance subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopBalance() {
    return noArgumentCommand("stopBalance");
  }

  /*!
   * Get request for candles subscription
   *
   * @param std::string* t_symbol
   * @return const char* t_streamSessionId
   */
  std::string RequestFactory::subscribeCandles(std::string* t_symbol, const char* t_streamSessionId) {
    Document document;

    SetValueByPointer(document, "/command", "getCandles");
    SetValueByPointer(document, "/streamSessionId", StringRef(t_streamSessionId));
    SetValueByPointer(document, "/symbol", StringRef(t_symbol->data()));

    return getStringify(&document);
  }

  /*!
   * Get request for candles
   *
   * @param std::string* t_symbol
   * @return std::string
   */
  std::string RequestFactory::stopCandles(std::string* t_symbol) {
    Document document;

    SetValueByPointer(document, "/command", "stopCandles");
    SetValueByPointer(document, "/symbol", StringRef(t_symbol->data()));

    return getStringify(&document);
  }

  /*!
   * Get request for keep alive
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeKeepAlive(const char* t_streamSessionId) {
    return startCommand("getKeepAlive", t_streamSessionId);
  }

  /*!
   * Get request to stop keep alive subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopKeepAlive() {
    return noArgumentCommand("stopKeepAlive");
  }

  /*!
   * Get request for news subscription
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeNews(const char* t_streamSessionId) {
    return startCommand("getNews", t_streamSessionId);
  }

  /*!
   * Get request to stop news subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopNews() {
    return noArgumentCommand("stopNews");
  }

  /*!
   * Get request for profits subscription
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeProfits(const char* t_streamSessionId) {
    return startCommand("getProfits", t_streamSessionId);
  }

  /*!
   * Get request to stop profits subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopProfits() {
    return noArgumentCommand("stopProfits");
  }


  /*!
   * Get request for tick prices subscription
   *
   * @param std::string* t_symbol
   * @param int t_minArrivalTime
   * @param int t_maxLevel
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTickPrices(std::string* t_symbol, int t_minArrivalTime, int t_maxLevel, const char* t_streamSessionId) {
    Document document;

    Value::AllocatorType& a = document.GetAllocator();
    SetValueByPointer(document, "/command", "getTickPrices", a);
    SetValueByPointer(document, "/streamSessionId", t_streamSessionId, a);
    SetValueByPointer(document, "/symbol", StringRef(t_symbol->c_str()), a);

    if( t_minArrivalTime > -1 ){
      SetValueByPointer(document, "/minArrivalTime", t_minArrivalTime, a);
    }

    if( t_maxLevel > -1 ){
      SetValueByPointer(document, "/maxLevel", t_maxLevel, a);
    }

    return getStringify(&document);
  }

  /*!
   * Get request to stop tick prices for symbol
   *
   * @param std::string* t_symbol
   * @return std::string
   */
  std::string RequestFactory::stopTickPrices(std::string* t_symbol) {
    Document document;

    SetValueByPointer(document, "/command", "stopTickPrices");
    SetValueByPointer(document, "/symbol", StringRef(t_symbol->data()));

    return getStringify(&document);
  }

  /*!
   * Get request for trades subscription
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTrades(const char* t_streamSessionId) {
    return startCommand("getTrades", t_streamSessionId);
  }

  /*!
   * Get request to stop trades subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopTrades() {
    return noArgumentCommand("stopTrades");
  }

  /*!
   * Get request for trade status subscription
   *
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTradeStatus(const char* t_streamSessionId) {
    return startCommand("getTradeStatus", t_streamSessionId);
  }

  /*!
   * Get request to stop trade status subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopTradeStatus() {
    return noArgumentCommand("stopTradeStatus");
  }

  /*!
   * Get start command for subscriptions
   *
   * @param std::string t_command
   * @param const char* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::startCommand(const char* t_command, const char* t_streamSessionId){
    Document document;

    SetValueByPointer(document, "/command", StringRef(t_command));
    SetValueByPointer(document, "/streamSessionId", StringRef(t_streamSessionId));

    return getStringify(&document);
  }

  /*!
   * Get single command without arguments
   *
   * @param const char* t_command
   * @return std::string
   */
  std::string RequestFactory::noArgumentCommand(const char* t_command){
    Document document;

    SetValueByPointer(document, "/command", StringRef(t_command));

    return getStringify(&document);
  }

  /*!
   * Get json stringify
   *
   * @param rapidjson::Document t_document
   * @return std::string
   */
  std::string RequestFactory::getStringify(rapidjson::Document *t_document) {
    StringBuffer stringBuffer;
    Writer<StringBuffer> writer(stringBuffer);
    t_document->Accept(writer);

    return stringBuffer.GetString();
  }
}
