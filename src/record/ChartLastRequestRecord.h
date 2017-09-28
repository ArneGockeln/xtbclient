//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_CHARTLASTREQUESTRECORD_H
#define XTBLIB_CHARTLASTREQUESTRECORD_H

#include <vector>
#include "RateInfoRecord.h"

namespace xtbclient {
  struct ChartLastRequestRecord {
    int m_digits;
    std::vector<RateInfoRecord> m_rateInfos;
  };
}

#endif //XTBLIB_CHARTLASTREQUESTRECORD_H
