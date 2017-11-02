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
    virtual void onDebugMsg(std::string msg);
  };
}


#endif //XTBCLIENT_STREAMLISTENER_H
