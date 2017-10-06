//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMTRADESTATUSRECORD_H
#define XTBCLIENT_STREAMTRADESTATUSRECORD_H

#include <string>
#include "../enums/RequestStatus.h"

namespace xtbclient {
  struct StreamTradeStatusRecord {
    std::string m_customComment;
    std::string m_message;
    unsigned int m_order;
    double m_price;
    RequestStatus m_requestStatus;
  };
}

#endif //XTBCLIENT_STREAMTRADESTATUSRECORD_H
