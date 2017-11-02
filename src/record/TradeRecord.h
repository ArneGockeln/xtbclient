/*!
 *  Copyright(c)2017, Arne Gockeln
 *  http://www.arnegockeln.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
