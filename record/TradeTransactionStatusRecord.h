//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_TRADETRANSACTIONSTATUSRECORD_H
#define XTBLIB_TRADETRANSACTIONSTATUSRECORD_H

#include <string>
#include "../enums/RequestStatus.h"

namespace xtblib {
  struct TradeTransactionStatusRecord{
    double m_ask;
    double m_bid;
    std::string m_customComment;
    std::string m_message;
    signed int m_order;
    RequestStatus m_requestStatus;
  };
}

#endif //XTBLIB_TRADETRANSACTIONSTATUSRECORD_H
