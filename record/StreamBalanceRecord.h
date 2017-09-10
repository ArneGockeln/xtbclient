//
// Created by Arne Gockeln on 17.07.17.
//

#ifndef XTBLIB_BALANCERECORD_H
#define XTBLIB_BALANCERECORD_H

#include <string>
#include <rapidjson/document.h>

using namespace rapidjson;

namespace xtbclient {
  struct StreamBalanceRecord {
    double m_balance;
    double m_credit;
    double m_equity;
    double m_margin;
    double m_marginFree;
    double m_marginLevel;
  };
}

#endif //XTBLIB_BALANCERECORD_H
