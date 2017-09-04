//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_STREAMLISTENER_H
#define XTBCLIENT_STREAMLISTENER_H

#include <string>

namespace xtbclient {
  class StreamListener {
  public:

    virtual void onBalance(std::string response);
    virtual void onCandles(std::string response);
    virtual void onKeepAlive(std::string response);
    virtual void onNews(std::string response);
    virtual void onProfits(std::string response);
    virtual void onTickPrices(std::string response);
    virtual void onTrades(std::string response);
    virtual void onTradeStatus(std::string response);
  };
}


#endif //XTBCLIENT_STREAMLISTENER_H
