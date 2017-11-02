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

#ifndef XTBLIB_IBRECORD_H
#define XTBLIB_IBRECORD_H

#include <string>
#include "../enums/Side.h"

namespace xtbclient {
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
