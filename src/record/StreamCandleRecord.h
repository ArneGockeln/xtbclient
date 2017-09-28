//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMCANDLERECORD_H
#define XTBCLIENT_STREAMCANDLERECORD_H

#include <string>
#include "../enums/QuoteId.h"

namespace xtbclient {
  struct StreamCandleRecord {
    double m_close;
    long long m_ctm;
    std::string m_ctmString;
    double m_high;
    double m_low;
    double m_open;
    QUOTEID m_quoteid;
    std::string m_symbol;
    double m_vol;
  };
}

#endif //XTBCLIENT_STREAMCANDLERECORD_H
