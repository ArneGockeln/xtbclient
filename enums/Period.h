//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_PERIOD_H
#define XTBLIB_PERIOD_H

namespace xtbclient {
  enum Period {
    PERIOD_M1 = 1,
    PERIOD_M5 = 5,
    PERIOD_M15 = 15,
    PERIOD_M30 = 30,
    PERIOD_H1 = 60,
    PERIOD_H4 = 240,
    PERIOD_D1 = 1440,
    PERIOD_W1 = 10080,
    PERIOD_MN1 = 43200
  };
}

#endif //XTBLIB_PERIOD_H
