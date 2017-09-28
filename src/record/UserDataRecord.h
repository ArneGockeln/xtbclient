//
// Created by Arne Gockeln on 08.07.17.
//

#ifndef XTBLIB_USERDATARECORD_H
#define XTBLIB_USERDATARECORD_H

#include <string>

namespace xtbclient {
  struct UserDataRecord {
    int m_companyUnit;
    std::string m_currency;
    std::string m_group;
    bool m_ibAccount;
    int m_leverage;
    double m_leverageMultiplier;
    std::string m_spreadType;
    bool m_trailingStop;
  };
}

#endif //XTBLIB_USERDATARECORD_H
