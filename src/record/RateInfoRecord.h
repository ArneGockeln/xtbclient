//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_RATEINFORECORD_H
#define XTBLIB_RATEINFORECORD_H

#include <cstdint>
#include <string>

namespace xtbclient {
  struct RateInfoRecord {
    double m_close;
    uint64_t m_ctm;
    std::string m_ctmString;
    double m_high;
    double m_low;
    double m_open;
    double m_vol;
  };
}

#endif //XTBLIB_RATEINFORECORD_H
