//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMTRADERECORD_H
#define XTBCLIENT_STREAMTRADERECORD_H

#include <string>
#include "../enums/TransactionCmd.h"
#include "../enums/TransactionType.h"

namespace xtbclient {
  struct StreamTradeRecord {
    double m_close_price;
    long long m_close_time;
    bool m_closed;
    TransactionCmd m_cmd;
    std::string m_comment;
    double m_commission;
    std::string m_customComment;
    int m_digits;
    long long m_expiration;
    double m_margin_rate;
    int m_offset;
    double m_open_price;
    long long m_open_time;
    unsigned long int m_order;
    unsigned long int m_order2;
    unsigned long int m_position;
    double m_profit;
    double m_sl;
    std::string m_state;
    double m_storage;
    std::string m_symbol;
    double m_tp;
    TransactionType m_type;
    double m_volume;
  };
}

#endif //XTBCLIENT_STREAMTRADERECORD_H
