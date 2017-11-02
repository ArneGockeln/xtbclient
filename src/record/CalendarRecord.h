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

#ifndef XTBLIB_CALENDARRECORD_H
#define XTBLIB_CALENDARRECORD_H

#include <string>

namespace xtbclient {
  struct CalendarRecord {
    std::string m_country;
    std::string m_current;
    std::string m_forecast;
    std::string m_impact;
    std::string m_period;
    std::string m_previous;
    uint64_t m_time;
    std::string m_title;
  };
}

#endif //XTBLIB_CALENDARRECORD_H
