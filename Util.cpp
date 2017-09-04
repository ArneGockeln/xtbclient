//
// Created by Arne Gockeln on 03.09.17.
//

#include "Util.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

using namespace rapidjson;

namespace xtbclient {
  /*!
   * Check for document parse error
   *
   * @param rapidjson::Document* t_document
   * @return bool
   */
  bool Util::hasDocumentParseError(Document *t_document) {
    if(t_document->HasParseError()){
      fprintf(stderr, "DocumentParseError: %s\n", GetParseError_En(t_document->GetParseError()));
      return true;
    }

    return false;
  }

  /*!
   * Check for api request error code
   *
   * @param std::string* jsonData
   */
  void Util::handleJsonResponseError(std::string* jsonData) {
    Document document;
    document.Parse(jsonData->c_str());

    if(document.HasParseError()){
      fprintf(stderr, "DocumentParseError: %s\n", GetParseError_En(document.GetParseError()));
      return;
    }

    // error handling
    if(document.HasMember("errorCode")){
      fprintf(stderr, "APIError: %s\n", document["errorCode"].GetString());
    }
  }

  // trim from start (in place)
   void Util::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
      return !std::isspace(ch);
    }));
  }

// trim from end (in place)
   void Util::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
      return !std::isspace(ch);
    }).base(), s.end());
  }

// trim from both ends (in place)
   void Util::trim(std::string &s) {
    ltrim(s);
    rtrim(s);
  }

// trim from start (copying)
   std::string Util::ltrim_copy(std::string s) {
    ltrim(s);
    return s;
  }

  // trim from end (copying)
   std::string Util::rtrim_copy(std::string s) {
    rtrim(s);
    return s;
  }

// trim from end (copying)
   std::string Util::trim_copy(std::string s) {
    rtrim(s);
    return s;
  }
}