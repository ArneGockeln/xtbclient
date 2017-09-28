//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_CHARTLASTINFORECORD_H
#define XTBLIB_CHARTLASTINFORECORD_H

#include <cstdint>
#include <string>
#include "../enums/Period.h"

namespace xtbclient {
  struct ChartLastInfoRecord {
    Period m_period;
    long long m_start;
    std::string m_symbol;
  };
}

#endif //XTBLIB_CHARTLASTINFORECORD_H
