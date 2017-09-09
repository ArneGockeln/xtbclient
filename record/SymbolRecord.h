/*
 * SymbolRecord.h
 *
 *  Created on: 10.06.2017
 *      Author: arnegockeln
 */

#ifndef SRC_XTB_RECORD_SYMBOLRECORD_H_
#define SRC_XTB_RECORD_SYMBOLRECORD_H_

#include "../enums/QuoteId.h"
#include "../enums/MarginMode.h"
#include "../enums/ProfitMode.h"
#include <rapidjson/document.h>
#include <stdio.h>

using namespace rapidjson;

namespace xtblib {

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
