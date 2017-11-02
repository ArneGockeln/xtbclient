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
