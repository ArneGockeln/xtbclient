//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_PROFITCMD_H
#define XTBLIB_PROFITCMD_H

namespace xtbclient {
  enum TransactionCmd {
    TC_BUY = 0,
    TC_SELL = 1,
    TC_BUY_LIMIT = 2,
    TC_SELL_LIMIT = 3,
    TC_BUY_STOP = 4,
    TC_SELL_STOP = 5,
    TC_BALANCE = 6,
    TC_CREDIT = 7
  };
}
#endif //XTBLIB_PROFITCMD_H
