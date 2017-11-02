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

#ifndef XTBCLIENT_STREAMTICKRECORD_H
#define XTBCLIENT_STREAMTICKRECORD_H

#include <string>
#include "../enums/QuoteId.h"

namespace xtbclient {
  struct StreamTickRecord {
    double m_ask;
    int m_askVolume;
    double m_bid;
    int m_bidVolume;
    double m_high;
    int m_level;
    double m_low;
    QUOTEID m_quoteid;
    double m_spreadRaw;
    double m_spreadTable;
    std::string m_symbol;
    long long m_timestamp;
  };
}

#endif //XTBCLIENT_STREAMTICKRECORD_H
