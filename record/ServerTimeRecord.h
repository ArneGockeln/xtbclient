//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_SERVERTIMERECORD_H
#define XTBLIB_SERVERTIMERECORD_H

#include <cstdint>
#include <string>

namespace xtblib {
  struct ServerTimeRecord {
    uint64_t m_time;
    std::string m_timeString;
  };
}

#endif //XTBLIB_SERVERTIMERECORD_H
