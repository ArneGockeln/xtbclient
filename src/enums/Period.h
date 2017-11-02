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

#ifndef XTBLIB_PERIOD_H
#define XTBLIB_PERIOD_H

namespace xtbclient {
  enum Period {
    PERIOD_M1 = 1,
    PERIOD_M5 = 5,
    PERIOD_M15 = 15,
    PERIOD_M30 = 30,
    PERIOD_H1 = 60,
    PERIOD_H4 = 240,
    PERIOD_D1 = 1440,
    PERIOD_W1 = 10080,
    PERIOD_MN1 = 43200
  };
}

#endif //XTBLIB_PERIOD_H
