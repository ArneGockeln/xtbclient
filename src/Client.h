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

#ifndef XTBCLIENT_CLIENT_H
#define XTBCLIENT_CLIENT_H

#include <rapidjson/error/en.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <functional>
#include <mutex>
#include "StreamListener.h"
#include "Util.h"
#include "record/ChartLastInfoRecord.h"
#include "record/ChartLastRequestRecord.h"
#include "record/SymbolRecord.h"
#include "record/CalendarRecord.h"
#include "record/ChartRangeInfoRecord.h"
#include "record/CommissionRecord.h"
#include "record/UserDataRecord.h"
#include "record/IBRecord.h"
#include "record/MarginLevelRecord.h"
#include "record/NewsRecord.h"
#include "enums/TransactionCmd.h"
#include "record/ServerTimeRecord.h"
#include "record/StepRuleRecord.h"
#include "record/TickRecord.h"
#include "record/TradeRecord.h"
#include "record/TradingHourRecord.h"
#include "record/TradeTransactionInfo.h"
#include "record/TradeTransactionStatusRecord.h"
#include <vector>
#include <cstdint>

namespace xtbclient {
  enum ClientType {
    DEMO,
    DEMO_STREAM,
    REAL,
    REAL_STREAM,
    LOCAL,
    LOCAL_STREAM
  };

  class Client {
  private:
    // the client type
    ClientType m_clientType;
    // pointer to ssl socket
    SSL* m_ssl = nullptr;
    // pointer to stream ssl socket for subscriptions
    SSL* m_ssl_stream = nullptr;
    // store stream session id for requests
    std::string m_streamSessionId;
    // store callback for single requests
    std::function<void(SSL*)> m_callback;
    // stream listener
    StreamListener *m_streamlistner;
    // mutex for ssl
    std::mutex m_ssl_mutex;
    // flag to enable stdout messages
    bool m_flag_verbose = false;
    // flag to enable json request output
    bool m_debug_json_requests = false;
    // flag to enable json response output
    bool m_debug_json_response = false;

    SSL_CTX* createContext();
    void initClient();
    SSL* getSSL();
    void setSSL(SSL* t_ssl);
    void setClientType(ClientType t_clientType);
    void ssl_write(SSL* t_ssl, const char* t_data, const int t_data_len);
    bool isResponseEnd(std::string t_buffer);
    void cleanResponse(std::string& t_response);
    Value* getReturnData(std::string t_json);
    Document getDocumentFromJson(std::string t_jsonString);
    const char* parseStreamSessionId(std::string t_jsonString);
    void listenOnStream();
    void parseBalance(Value*);
    void parseTickPrices(Value*);
    void parseCandles(Value*);
    void parseKeepAlive(Value*);
    void parseNews(Value*);
    void parseProfits(Value*);
    void parseTrades(Value*);
    void parseTradeStatus(Value*);

  public:
    Client(ClientType t_clientType);
    ~Client();

    bool login(const char *t_username, const char *t_password);
    void logout();
    std::string sendRequest(std::string t_json);
    void sendRequestWithoutResponse(std::string t_json);
    std::string getResponse();
    void setStreamListener(StreamListener* t_streamListener);
    const char* getStreamSessionId();
    void setStreamSessionId(const char* t_streamSessionId);

    void setDebugJsonRequests(bool flag);
    void setDebugJsonResponse(bool flag);
    void setDebugOutput(bool flag_requests, bool flag_responses);
    void setVerbose(bool flag);

    /*!
     * Requests
     */
    std::vector<SymbolRecord> getAllSymbols();
    ChartLastRequestRecord getChartLastRequest( ChartLastInfoRecord& t_record );
    std::vector<CalendarRecord> getCalendar();
    ChartLastRequestRecord getChartRangeRequest(ChartRangeInfoRecord &t_record);
    CommissionRecord getCommissionDef( const std::string& t_symbol, double t_volume );
    UserDataRecord getCurrentUserData();
    std::vector<IBRecord> getIbsHistory( long long t_start, long long t_end );
    MarginLevelRecord getMarginLevel();
    double getMarginTrade(const std::string& t_symbol, double t_volume);
    std::vector<NewsRecord> getNews(long long t_start, long long t_end);
    double getProfitCalculation(double t_closePrice, double t_openPrice, TransactionCmd t_cmd, const std::string& t_symbol, double t_volume);
    ServerTimeRecord getServerTime();
    std::vector<StepRuleRecord> getStepRules();
    SymbolRecord getSymbol(const std::string& t_symbol);
    std::vector<TickRecord> getTickPrices(int t_level, long long t_timestamp, std::vector<std::string> t_symbols);
    std::vector<TradeRecord> getTradeRecords(std::vector<unsigned long int> t_orders);
    std::vector<TradeRecord> getTrades(bool t_openedOnly);
    std::vector<TradeRecord> getTradeHistory(long long t_start, long long t_end);
    std::vector<TradingHourRecord> getTradingHours(std::vector<std::string> t_symbols);
    std::string getVersion();
    void ping();
    unsigned long int tradeTransaction(TradeTransactionInfo &t_info);
    TradeTransactionStatusRecord getTradeTransactionStatus(unsigned long int t_ordernr);
    TradeRecord getTradeRecordFromObj(const rapidjson::GenericValue<rapidjson::UTF8<char>>& obj);

    /*!
     * Subscriptions
     */
    void subscribeBalance();
    void stopBalance();
    void subscribeCandles(std::string t_symbol);
    void stopCandles(std::string t_symbol);
    void subscribeKeepAlive();
    void stopKeepAlive();
    void subscribeNews();
    void stopNews();
    void subscribeProfits();
    void stopProfits();
    void subscribeTickPrices(std::string t_symbol, int t_minArrivalTime = -1, int t_maxLevel = -1);
    void stopTickPrices(std::string t_symbol);
    void subscribeTrades();
    void stopTrades();
    void subscribeTradeStatus();
    void stopTradeStatus();
  };
}


#endif //XTBCLIENT_CLIENT_H
