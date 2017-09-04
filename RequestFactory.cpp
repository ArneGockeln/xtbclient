//
// Created by Arne Gockeln on 02.09.17.
//

#include "RequestFactory.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

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
    std::string cmd = "getAllSymbols";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get calendar request
   *
   * @return std::string
   */
  std::string RequestFactory::getCalendar() {
    std::string cmd = "getCalendar";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get request for balance subscription
   *
   * @param std::string* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeBalance(std::string *t_streamSessionId) {
    std::string cmd = "getBalance";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop balance subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopBalance() {
    std::string cmd = "stopBalance";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get request for candles subscription
   *
   * @param std::string* t_symbol
   * @return std::string
   */
  std::string RequestFactory::subscribeCandles(std::string* t_symbol, std::string* t_streamSessionId) {
    Document document;

    SetValueByPointer(document, "/command", "getCandles");
    SetValueByPointer(document, "/streamSessionId", StringRef(t_streamSessionId->data()));
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
   * @param std::string* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeKeepAlive(std::string* t_streamSessionId) {
    std::string cmd = "getKeepAlive";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop keep alive subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopKeepAlive() {
    std::string cmd = "stopKeepAlive";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get request for news subscription
   *
   * @param std::string* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeNews(std::string* t_streamSessionId) {
    std::string cmd = "getNews";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop news subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopNews() {
    std::string cmd = "stopNews";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get request for profits subscription
   *
   * @param std::string t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeProfits(std::string* t_streamSessionId) {
    std::string cmd = "getProfits";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop profits subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopProfits() {
    std::string cmd = "stopProfits";
    return noArgumentCommand(&cmd);
  }


  /*!
   * Get request for tick prices subscription
   *
   * @param std::string* t_symbol
   * @param int t_minArrivalTime
   * @param int t_maxLevel
   * @param std::string* t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTickPrices(std::string* t_symbol, int t_minArrivalTime, int t_maxLevel, std::string* t_streamSessionId) {
    Document document;

    SetValueByPointer(document, "/command", "getTickPrices");
    SetValueByPointer(document, "/streamSessionId", StringRef(t_streamSessionId->data()));
    SetValueByPointer(document, "/symbol", StringRef(t_symbol->data()));
    SetValueByPointer(document, "/minArrivalTime", t_minArrivalTime);
    SetValueByPointer(document, "/maxLevel", t_maxLevel);

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
   * @param std::string t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTrades(std::string* t_streamSessionId) {
    std::string cmd = "getTrades";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop trades subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopTrades() {
    std::string cmd = "stopTrades";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get request for trade status subscription
   *
   * @param std::string t_streamSessionId
   * @return std::string
   */
  std::string RequestFactory::subscribeTradeStatus(std::string* t_streamSessionId) {
    std::string cmd = "getTradeStatus";
    return startCommand(&cmd, t_streamSessionId);
  }

  /*!
   * Get request to stop trade status subscription
   *
   * @return std::string
   */
  std::string RequestFactory::stopTradeStatus() {
    std::string cmd = "stopTradeStatus";
    return noArgumentCommand(&cmd);
  }

  /*!
   * Get start command for subscriptions
   *
   * @param std::string t_command
   * @return std::string
   */
  std::string RequestFactory::startCommand(std::string* t_command, std::string* t_streamSessionId){
    Document document;

    SetValueByPointer(document, "/command", StringRef(t_command->data()));
    SetValueByPointer(document, "/streamSessionId", StringRef(t_streamSessionId->data()));

    return getStringify(&document);
  }

  /*!
   * Get single command without arguments
   *
   * @param std::string t_command
   * @return std::string
   */
  std::string RequestFactory::noArgumentCommand(std::string *t_command){
    Document document;

    SetValueByPointer(document, "/command", StringRef(t_command->data()));

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