//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_TRANSACTIONTYPE_H
#define XTBLIB_TRANSACTIONTYPE_H

namespace xtbclient {
  enum TransactionType {
    TT_OPEN, TT_PENDING, TT_CLOSE, TT_MODIFY, TT_DELETE
  };
}

#endif //XTBLIB_TRANSACTIONTYPE_H
