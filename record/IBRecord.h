//
// Created by Arne Gockeln on 08.07.17.
//

#ifndef XTBLIB_IBRECORD_H
#define XTBLIB_IBRECORD_H

#include <string>
#include "../enums/Side.h"

namespace xtblib {
  struct IBRecord {
    double m_closePrice;
    std::string m_login;
    double m_nominal;
    double m_openPrice;
    Side m_side;
    std::string m_surname;
    std::string m_symbol;
    uint64_t m_timestamp;
    double m_volume;
  };
}
#endif //XTBLIB_IBRECORD_H
