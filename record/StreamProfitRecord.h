//
// Created by Arne Gockeln on 10.09.17.
//

#ifndef XTBCLIENT_STREAMPROFITRECORD_H
#define XTBCLIENT_STREAMPROFITRECORD_H

namespace xtbclient {
  struct StreamProfitRecord {
    int m_order;
    int m_order2;
    int m_position;
    double m_profit;
  };
}

#endif //XTBCLIENT_STREAMPROFITRECORD_H
