//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_RESPONSEFACTORY_H
#define XTBCLIENT_RESPONSEFACTORY_H

#include <string>
#include "Client.h"
#include <rapidjson/document.h>
#include "endpoints/ChartLastRequest.h"

namespace xtbclient {

  using namespace rapidjson;

  class ResponseFactory {
  private:
    static Document getDocumentFromJson(std::string t_jsonString);
    static Value* getReturnData(std::string t_json);

  public:
    static std::string getStreamSessionId(std::string t_jsonString);
    static ChartLastRequest getChartLastRequest(std::string t_json);
  };
}


#endif //XTBCLIENT_RESPONSEFACTORY_H
