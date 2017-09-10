//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_TRADETRANSACTIONINFO_H
#define XTBLIB_TRADETRANSACTIONINFO_H

#include <string>
#include "../enums/TransactionCmd.h"
#include "../enums/TransactionType.h"

namespace xtbclient {
  struct TradeTransactionInfo {
    TransactionCmd m_cmd;
    std::string m_customComment;
    uint64_t m_expiration;
    signed int m_offset;
    signed int m_order;
    double m_price;
    double m_sl;
    std::string m_symbol;
    double m_tp;
    TransactionType m_type;
    double m_volume;
  };
}

#endif //XTBLIB_TRADETRANSACTIONINFO_H
