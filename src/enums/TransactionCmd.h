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

#ifndef XTBLIB_PROFITCMD_H
#define XTBLIB_PROFITCMD_H

namespace xtbclient {
  enum TransactionCmd {
    TC_BUY = 0,
    TC_SELL = 1,
    TC_BUY_LIMIT = 2,
    TC_SELL_LIMIT = 3,
    TC_BUY_STOP = 4,
    TC_SELL_STOP = 5,
    TC_BALANCE = 6,
    TC_CREDIT = 7
  };
}
#endif //XTBLIB_PROFITCMD_H
