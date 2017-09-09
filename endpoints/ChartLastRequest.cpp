//
// Created by Arne Gockeln on 09.09.17.
//

#include "ChartLastRequest.h"

xtbclient::ChartLastRequest::ChartLastRequest() {
  m_digits = 0;
}

xtbclient::ChartLastRequest::~ChartLastRequest() {

}

void xtbclient::ChartLastRequest::setDigits(int t_digits) {
  m_digits = t_digits;
}

int xtbclient::ChartLastRequest::getDigits() {
  return m_digits;
}

void xtbclient::ChartLastRequest::setRateInfos(std::vector<xtbclient::RateInfoRecord> t_rate_infos) {
  m_rate_infos = t_rate_infos;
}

std::vector<xtbclient::RateInfoRecord> xtbclient::ChartLastRequest::getRateInfos() {
  return m_rate_infos;
}
