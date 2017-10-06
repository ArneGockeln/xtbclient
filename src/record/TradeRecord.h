//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_TRADERECORD_H
#define XTBLIB_TRADERECORD_H

#include <cstdint>
#include <string>
#include "../enums/TransactionCmd.h"

namespace xtbclient {
  struct TradeRecord {
    double m_close_price;
    uint64_t m_close_time;
    std::string m_close_timeString;
    bool m_closed;
    TransactionCmd m_cmd;
    std::string m_comment;
    double m_commission;
    std::string m_customComment;
    int m_digits;
    uint64_t m_expiration;
    std::string m_expirationString;
    double m_margin_rate;
    int m_offset;
    double m_open_price;
    uint64_t m_open_time;
    std::string m_open_timeString;
    unsigned long int m_order;
    unsigned long int m_order2;
    unsigned long int m_position;
    double m_profit;
    double m_sl;
    double m_storage;
    std::string m_symbol;
    uint64_t m_timestamp;
    double m_tp;
    double m_volume;
  };
}

#endif //XTBLIB_TRADERECORD_H
