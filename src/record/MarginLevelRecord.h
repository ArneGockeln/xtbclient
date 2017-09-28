//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_MARGINLEVELRECORD_H
#define XTBLIB_MARGINLEVELRECORD_H

#include <string>

namespace xtbclient {
  struct MarginLevelRecord {
    double m_balance;
    double m_credit;
    std::string m_currency;
    double m_equity;
    double m_margin;
    double m_margin_free;
    double m_margin_level;
  };
}

#endif //XTBLIB_MARGINLEVELRECORD_H
