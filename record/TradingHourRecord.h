//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_TRADINGHOURRECORD_H
#define XTBLIB_TRADINGHOURRECORD_H


#include <string>
#include <vector>
#include "QuoteRecord.h"
#include "TradingRecord.h"

namespace xtblib {
  struct TradingHourRecord {
    std::vector<QuoteRecord> m_quotes;
    std::string m_symbol;
    std::vector<TradingRecord> m_trading;
  };
}

#endif //XTBLIB_TRADINGHOURRECORD_H
