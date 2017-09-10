//
// Created by Arne Gockeln on 09.09.17.
//

#ifndef XTBCLIENT_CHARTLASTREQUEST_H
#define XTBCLIENT_CHARTLASTREQUEST_H


#include <vector>
#include "record/RateInfoRecord.h"

namespace xtbclient {
  class ChartLastRequest {
  private:
    int m_digits;
    std::vector<RateInfoRecord> m_rate_infos;

  public:
    ChartLastRequest();
    ~ChartLastRequest();

    void setDigits(int t_digits);
    int getDigits();
    void setRateInfos(std::vector<RateInfoRecord> t_rate_infos);
    std::vector<RateInfoRecord> getRateInfos();
  };
}


#endif //XTBCLIENT_CHARTLASTREQUEST_H
