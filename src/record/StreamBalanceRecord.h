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

#ifndef XTBLIB_BALANCERECORD_H
#define XTBLIB_BALANCERECORD_H

#include <string>
#include <rapidjson/document.h>

using namespace rapidjson;

namespace xtbclient {
  struct StreamBalanceRecord {
    double m_balance;
    double m_credit;
    double m_equity;
    double m_margin;
    double m_marginFree;
    double m_marginLevel;
  };
}

#endif //XTBLIB_BALANCERECORD_H
