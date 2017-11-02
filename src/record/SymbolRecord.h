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

#ifndef SRC_XTB_RECORD_SYMBOLRECORD_H_
#define SRC_XTB_RECORD_SYMBOLRECORD_H_

#include "../enums/QuoteId.h"
#include "../enums/MarginMode.h"
#include "../enums/ProfitMode.h"
#include <rapidjson/document.h>
#include <string>

using namespace rapidjson;

namespace xtbclient {

  struct SymbolRecord {
    double m_ask;
    double m_bid;
	  std::string m_categoryName;
    unsigned int m_contractSize;
    std::string m_currency;
    bool m_currencyPair;
    std::string m_currencyProfit;
    std::string m_description;
    uint64_t m_expiration;
    std::string m_groupName;
    double m_high;
    signed int m_initialMargin;
    unsigned int m_instantMaxVolume;
    double m_leverage;
    bool m_longOnly;
    double m_lotMax;
    double m_lotMin;
    double m_lotStep;
    double m_low;
    int m_marginHedged;
    bool m_marginHedgedStrong;
    int m_marginMaintenance;
    MARGIN_MODE m_marginMode;
    double m_percentage;
    int m_precision;
    PROFIT_MODE m_profitMode;
    QUOTEID m_quoteId;
    bool m_shortSelling;
    double m_spreadRaw;
    double m_spreadTable;
    uint64_t m_starting;
    int m_stepRuleId;
    int m_stopsLevel;
    int m_swap_rollover3days;
    bool m_swapEnable;
    double m_swapLong;
    double m_swapShort;
    int m_swapType;
    std::string m_symbol;
    double m_tickSize;
    double m_tickValue;
    uint64_t m_time;
    std::string m_timeString;
    bool m_trailingEnabled;
    int m_type;
  };

}
#endif /* SRC_XTB_RECORD_SYMBOLRECORD_H_ */
