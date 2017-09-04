//
// Created by Arne Gockeln on 02.09.17.
//

#include "ResponseFactory.h"

namespace xtbclient {
  /*!
   * Get stream session id
   *
   * @param Client* t_client
   * @return std::string
   */
  std::string ResponseFactory::getStreamSessionId(Client *t_client) {
    Document document = getDocument(t_client);

    std::string sessionId;
    if(document.HasParseError()){
      return sessionId;
    }

    if(!document["status"].IsNull()){
      if(document["status"].GetBool()){
        sessionId = static_cast<std::string>(document["streamSessionId"].GetString());
      }
    }

    return sessionId;
  }

  /*!
   * Print json response for debugging
   *
   * @param Client* t_client
   */
  void ResponseFactory::printJsonResponse(Client* t_client){
    if(t_client->getResponse().length() > 0){
      printf("Debug: %s\n", t_client->getResponse().data());
    }
  }

  /*!
   * Parse json to rapidjson document
   *
   * @param Client* t_client
   * @return rapidjson::Document
   */
  Document ResponseFactory::getDocument(Client* t_client){
    Document document;
    document.Parse(t_client->getResponse().data());

    if(Util::hasDocumentParseError(&document)){
      fprintf(stderr, "CorruptJsonString: %s\n", t_client->getResponse().data());
    }

    return document;
  }
}