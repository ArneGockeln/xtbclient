//
// Created by Arne Gockeln on 02.09.17.
//

#include "StreamListener.h"

namespace xtbclient {
  void StreamListener::onBalance(StreamBalanceRecord balanceRecord) {
    printf("onBalance is not implemented yet.");
  }

  void StreamListener::onCandle(StreamCandleRecord candleRecord) {
    printf("onCandles is not implemented yet.");
  }

  void StreamListener::onKeepAlive(long long timestamp) {
    printf("onKeepAlive is not implemented yet.");
  }

  void StreamListener::onNews(StreamNewsRecord newsRecord) {
    printf("onNews is not implemented yet.");
  }

  void StreamListener::onProfits(StreamProfitRecord profitRecord) {
    printf("onProfits is not implemented yet.");
  }

  void StreamListener::onTickPrices(StreamTickRecord tickRecord) {
    printf("onTickPrices is not implemented yet.");
  }

  void StreamListener::onTrades(StreamTradeRecord tradeRecord) {
    printf("onTrades is not implemented yet.");
  }

  void StreamListener::onTradeStatus(StreamTradeStatusRecord tradeStatusRecord) {
    printf("onTradeStatus is not implemented yet.");
  }
};