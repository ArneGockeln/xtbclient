//
// Created by Arne Gockeln on 02.09.17.
//

#include "StreamListener.h"

namespace xtbclient {
  void StreamListener::onBalance(std::string response) {
    printf("onBalance is not implemented yet.");
  }

  void StreamListener::onCandles(std::string response) {
    printf("onCandles is not implemented yet.");
  }

  void StreamListener::onKeepAlive(std::string response) {
    printf("onKeepAlive is not implemented yet.");
  }

  void StreamListener::onNews(std::string response) {
    printf("onNews is not implemented yet.");
  }

  void StreamListener::onProfits(std::string response) {
    printf("onProfits is not implemented yet.");
  }

  void StreamListener::onTickPrices(std::string response) {
    printf("onTickPrices is not implemented yet.");
  }

  void StreamListener::onTrades(std::string response) {
    printf("onTrades is not implemented yet.");
  }

  void StreamListener::onTradeStatus(std::string response) {
    printf("onTradeStatus is not implemented yet.");
  }
};