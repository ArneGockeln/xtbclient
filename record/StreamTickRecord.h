//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMTICKRECORD_H
#define XTBCLIENT_STREAMTICKRECORD_H

#include <string>
#include "../enums/QuoteId.h"

namespace xtbclient {
  struct StreamTickRecord {
    double m_ask;
    int m_askVolume;
    double m_bid;
    int m_bidVolume;
    double m_high;
    int m_level;
    double m_low;
    QUOTEID m_quoteid;
    double m_spreadRaw;
    double m_spreadTable;
    std::string m_symbol;
    long long m_timestamp;
  };
}

#endif //XTBCLIENT_STREAMTICKRECORD_H
