//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_TRADINGRECORD_H
#define XTBLIB_TRADINGRECORD_H

#include <cstdint>
#include "../enums/Weekdays.h"

namespace xtbclient {
  struct TradingRecord {
    Weekdays m_day;
    uint64_t m_fromT;
    uint64_t m_toT;
  };
}

#endif //XTBLIB_TRADINGRECORD_H
