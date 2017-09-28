//
// Created by Arne Gockeln on 14.07.17.
//

#ifndef XTBLIB_STEPRULERECORD_H
#define XTBLIB_STEPRULERECORD_H

#include <string>
#include <vector>
#include "StepRecord.h"

namespace xtbclient {
  struct StepRuleRecord {
    int m_id;
    std::string m_name;
    std::vector<StepRecord> m_steps;
  };
}

#endif //XTBLIB_STEPRULERECORD_H
