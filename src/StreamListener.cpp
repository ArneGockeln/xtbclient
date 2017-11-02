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

  void StreamListener::onDebugMsg(std::string msg) {
    printf("StreamListener Debug: %s\n", msg.c_str());
  }
};