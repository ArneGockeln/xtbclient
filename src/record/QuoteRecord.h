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

#ifndef XTBLIB_QUOTERECORD_H
#define XTBLIB_QUOTERECORD_H

#include <cstdint>
#include "../enums/Weekdays.h"

namespace xtbclient {
 struct QuoteRecord {
   Weekdays m_day;
   uint64_t m_fromT;
   uint64_t m_toT;
 };
}

#endif //XTBLIB_QUOTERECORD_H
