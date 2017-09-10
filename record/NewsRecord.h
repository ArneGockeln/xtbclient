//
// Created by Arne Gockeln on 14.07.17.
//

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
