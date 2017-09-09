//
// Created by Arne Gockeln on 02.09.17.
//

#include "ResponseFactory.h"

namespace xtbclient {
  /*!
   * Get stream session id
   *
   * @param std::string t_jsonString
   * @return std::string
   */
  std::string ResponseFactory::getStreamSessionId(std::string t_jsonString) {
    Document document = getDocumentFromJson(std::move(t_jsonString));

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
   * Parse json to rapidjson document
   *
   * @param std::string t_jsonString
   * @return rapidjson::Document
   */
  Document ResponseFactory::getDocumentFromJson(std::string t_jsonString){

    Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(t_jsonString.c_str());

    if(Util::hasDocumentParseError(&document)){
      fprintf(stderr, "CorruptJsonString: %s\n", t_jsonString.c_str());
    }

    return document;
  }
}