//
// Created by Arne Gockeln on 15.07.17.
//

#ifndef XTBLIB_REQUESTSTATUS_H
#define XTBLIB_REQUESTSTATUS_H

namespace xtbclient {
  enum RequestStatus {
    RS_ERROR, RS_PENDING, RS_ACCEPTED, RS_REJECTED
  };
}

#endif //XTBLIB_REQUESTSTATUS_H
