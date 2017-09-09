//
// Created by Arne Gockeln on 29.06.17.
//

#ifndef XTBLIB_CALENDARRECORD_H
#define XTBLIB_CALENDARRECORD_H

#include <string>

namespace xtblib {
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
