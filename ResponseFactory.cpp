//
// Created by Arne Gockeln on 02.09.17.
//

#include "ResponseFactory.h"
#include "endpoints/ChartLastRequest.h"

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

  /*!
   * Get returnData from json string
   *
   * @param std::string t_json
   * @return rapidjson::Value*
   */
  Value* ResponseFactory::getReturnData(std::string t_json) {
    Document document = getDocumentFromJson(std::move(t_json));
    Value* value;

    if(document.HasParseError()){
      return value;
    }

    // has error?
    if(document.HasMember("errorCode")){
      fprintf(stderr, "API Error %s: %s\n", document["errorCode"].GetString(), document["errorDescr"].GetString());
    } else if(!document.HasMember("returnData") || !document.HasMember("status")){
      fprintf(stderr, "No returnData found.\n");
    } else if(!document["status"].IsNull()) {
      if (document["status"].GetBool()) {
        if(!document["returnData"].IsNull()){
           value = &document["returnData"];
        }
      }
    }

    return value;
  }


  /*!
   * Get ChartLastRequest Object from json
   *
   * @param std::string t_json
   * @return ChartLastRequest
   */
  ChartLastRequest ResponseFactory::getChartLastRequest(std::string t_json) {

    ChartLastRequest lastRequest;
    if(Util::hasAPIResponseError(t_json)){
      return lastRequest;
    }

    try {
      auto returnData = getReturnData(std::move(t_json))->GetObject();

      if(!returnData["digits"].IsNull()){
        lastRequest.setDigits( returnData["digits"].GetInt() );
      }

      if(!returnData["rateInfos"].IsNull() && returnData["rateInfos"].IsArray()){
        for(auto& rateInfo : returnData["rateInfos"].GetArray()){
          RateInfoRecord rateInfoRecord;

          if(!rateInfo["close"].IsNull()){
            rateInfoRecord.m_close = rateInfo["close"].GetDouble();
          }
          if(!rateInfo["ctm"].IsNull()){
            rateInfoRecord.m_ctm = rateInfo["ctm"].GetUint64();
          }
          if(!rateInfo["ctmString"].IsNull()){
            rateInfoRecord.m_ctmString = rateInfo["ctmString"].GetString();
          }
          if(!rateInfo["high"].IsNull()){
            rateInfoRecord.m_high = rateInfo["high"].GetDouble();
          }
          if(!rateInfo["low"].IsNull()){
            rateInfoRecord.m_low = rateInfo["low"].GetDouble();
          }
          if(!rateInfo["open"].IsNull()){
            rateInfoRecord.m_open = rateInfo["open"].GetDouble();
          }
          if(!rateInfo["vol"].IsNull()){
            rateInfoRecord.m_vol = rateInfo["vol"].GetDouble();
          }

          lastRequest.getRateInfos().push_back(rateInfoRecord);
        }
      }
    } catch(...){
      fprintf(stderr, "unknown error in ResponseFactory::getChartLastRequest\n");
    }

    return lastRequest;
  }
}