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
    unsigned long int m_order;
    double m_price;
    double m_sl;
    std::string m_symbol;
    double m_tp;
    TransactionType m_type;
    double m_volume;
  };
}

#endif //XTBLIB_TRADETRANSACTIONINFO_H
