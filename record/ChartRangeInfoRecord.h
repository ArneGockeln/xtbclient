//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_CHARTRANGEINFORECORD_H
#define XTBLIB_CHARTRANGEINFORECORD_H

#include <cstdint>
#include <string>
#include "../enums/Period.h"

namespace xtblib {
  struct ChartRangeInfoRecord {
    uint64_t m_end;
    Period m_period;
    uint64_t m_start;
    std::string m_symbol;
    signed int m_ticks;
  };
}

#endif //XTBLIB_CHARTRANGEINFORECORD_H
