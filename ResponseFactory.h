//
// Created by Arne Gockeln on 02.09.17.
//

#ifndef XTBCLIENT_RESPONSEFACTORY_H
#define XTBCLIENT_RESPONSEFACTORY_H

#include <string>
#include "Client.h"
#include <rapidjson/document.h>

namespace xtbclient {

  using namespace rapidjson;

  class ResponseFactory {
  private:
    static Document getDocument(Client* t_client);
    static void printJsonResponse(Client* t_client);

  public:
    static std::string getStreamSessionId(Client *t_client);
  };
}


#endif //XTBCLIENT_RESPONSEFACTORY_H
