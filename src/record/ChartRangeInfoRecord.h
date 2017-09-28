//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_CHARTRANGEINFORECORD_H
#define XTBLIB_CHARTRANGEINFORECORD_H

#include <cstdint>
#include <string>
#include "../enums/Period.h"

namespace xtbclient {
  struct ChartRangeInfoRecord {
    long long int m_end;
    Period m_period;
    long long int m_start;
    std::string m_symbol;
    signed int m_ticks;
  };
}

#endif //XTBLIB_CHARTRANGEINFORECORD_H
