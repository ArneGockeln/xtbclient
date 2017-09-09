//
// Created by Arne Gockeln on 17.07.17.
//

#ifndef XTBLIB_BALANCERECORD_H
#define XTBLIB_BALANCERECORD_H

#include <string>
#include <rapidjson/document.h>

using namespace rapidjson;

namespace xtblib {
  struct BalanceRecord {
    double m_balance;
    double m_credit;
    double m_equity;
    double m_margin;
    double m_marginFree;
    double m_marginLevel;

  public:
    /**
     * Parse json
     *
     * @param t_json
     */
    void parse(const std::string* t_json){
      Document document;
      document.Parse(t_json->c_str());

      if(!document["balance"].IsNull()){
        m_balance = document["balance"].GetDouble();
      }
      if(!document["credit"].IsNull()){
        m_credit = document["credit"].GetDouble();
      }
      if(!document["equity"].IsNull()){
        m_equity = document["equity"].GetDouble();
      }
      if(!document["margin"].IsNull()){
        m_margin = document["margin"].GetDouble();
      }
      if(!document["marginFree"].IsNull()){
        m_marginFree = document["marginFree"].GetDouble();
      }
      if(!document["marginLevel"].IsNull()){
        m_marginLevel = document["marginLevel"].GetDouble();
      }
    }
  };
}

#endif //XTBLIB_BALANCERECORD_H
