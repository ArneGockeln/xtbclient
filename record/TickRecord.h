//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_TICKRECORD_H
#define XTBLIB_TICKRECORD_H

#include <string>

namespace xtblib {
  struct TickRecord {
    double m_ask;
    int m_askVolume;
    double m_bid;
    int m_bidVolume;
    double m_high;
    int m_level;
    double m_low;
    double m_spreadRaw;
    double m_spreadTable;
    std::string m_symbol;
    uint64_t m_timestamp;
  };
}

#endif //XTBLIB_TICKRECORD_H
