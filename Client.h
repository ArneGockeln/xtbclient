//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_CLIENT_H
#define XTBCLIENT_CLIENT_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <string>
#include <functional>
#include <mutex>
#include "StreamListener.h"
#include "Util.h"
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
    std::string* m_streamSessionId;
    // store callback for single requests
    std::function<void(SSL*)> m_callback;
    // stream listener
    StreamListener *m_streamlistner;
    // mutex for ssl
    std::mutex m_ssl_mutex;

    SSL_CTX* createContext();
    void initClient();
    SSL* getSSL();
    void setSSL(SSL* t_ssl);
    void setClientType(ClientType t_clientType);
    void startFork(std::function<void(std::string)> t_callback);
    void ssl_write(SSL* t_ssl, const char* t_data, const int t_data_len);
    int ssl_read(SSL* t_ssl, void* t_buffer, int t_buffer_size);
    bool is_response_end(std::string t_buffer);
    void cleanResponse(std::string& t_response);

  public:
    Client(ClientType t_clientType);
    ~Client();

    bool sendLogin(const char* t_username, const char* t_password);
    std::string sendRequest(std::string t_json);
    std::string getResponse();

    void setStreamListener(StreamListener* t_streamListener);
    std::string* getStreamSessionId();
    void setStreamSessionId(std::string* t_streamSessionId);

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
    void subscribeTickPrices(std::string t_symbol, int t_minArrivalTime = 0, int t_maxLevel = -1);
    void stopTickPrices(std::string t_symbol);
    void subscribeTrades();
    void stopTrades();
    void subscribeTradeStatus();
    void stopTradeStatus();
  };
}


#endif //XTBCLIENT_CLIENT_H
