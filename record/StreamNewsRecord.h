//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMNEWSRECORD_H
#define XTBCLIENT_STREAMNEWSRECORD_H

#include <string>

namespace xtbclient {
  struct StreamNewsRecord {
    std::string m_body;
    std::string m_key;
    long long m_time;
    std::string m_title;
  };
}

#endif //XTBCLIENT_STREAMNEWSRECORD_H
