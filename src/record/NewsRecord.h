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

#ifndef XTBLIB_NEWSRECORD_H
#define XTBLIB_NEWSRECORD_H

#include <string>

namespace xtbclient {
  struct NewsRecord {
    std::string m_body;
    int m_bodylen;
    std::string m_key;
    uint64_t m_time;
    std::string m_timeString;
    std::string m_title;
  };
}

#endif //XTBLIB_NEWSRECORD_H
