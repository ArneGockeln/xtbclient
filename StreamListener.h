//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_STREAMLISTENER_H
#define XTBCLIENT_STREAMLISTENER_H

#include <string>
#include "record/StreamBalanceRecord.h"
#include "record/StreamCandleRecord.h"
#include "record/StreamNewsRecord.h"
#include "record/StreamProfitRecord.h"
#include "record/StreamTickRecord.h"
#include "record/StreamTradeRecord.h"
#include "record/StreamTradeStatusRecord.h"

namespace xtbclient {
  class StreamListener {
  public:

    virtual void onBalance(StreamBalanceRecord balanceRecord);
    virtual void onCandle(StreamCandleRecord candleRecord);
    virtual void onKeepAlive(long long timestamp);
    virtual void onNews(StreamNewsRecord newsRecord);
    virtual void onProfits(StreamProfitRecord profitRecord);
    virtual void onTickPrices(StreamTickRecord tickRecord);
    virtual void onTrades(StreamTradeRecord tradeRecord);
    virtual void onTradeStatus(StreamTradeStatusRecord statusRecord);
  };
}


#endif //XTBCLIENT_STREAMLISTENER_H
