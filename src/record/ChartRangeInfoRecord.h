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

#ifndef XTBLIB_CHARTRANGEINFORECORD_H
#define XTBLIB_CHARTRANGEINFORECORD_H

#include <cstdint>
#include <string>
#include "../enums/Period.h"

namespace xtbclient {
  struct ChartRangeInfoRecord {
    long long int m_end;
    Period m_period;
    long long int m_start;
    std::string m_symbol;
    signed int m_ticks;
  };
}

#endif //XTBLIB_CHARTRANGEINFORECORD_H
