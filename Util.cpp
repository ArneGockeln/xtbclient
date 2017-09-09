//
// Created by Arne Gockeln on 03.09.17.
//

#include "Util.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "date/date.h"
#include <chrono>

using namespace rapidjson;
using namespace date;
using namespace std::chrono;

namespace xtbclient {
  /*!
   * Check for document parse error
   *
   * @param rapidjson::Document* t_document
   * @return bool
   */
  bool Util::hasDocumentParseError(Document *t_document) {
    if(t_document->HasParseError()){
      fprintf(stderr, "JSONParseError: %s\n", GetParseError_En(t_document->GetParseError()));
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
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(jsonData->c_str());

    if(document.HasParseError()){
      fprintf(stderr, "JSONParseError: %s\n", GetParseError_En(document.GetParseError()));
      return;
    }

    // error handling
    hasAPIResponseError(*jsonData);
  }

  /*!
   * Check for api request error code
   *
   * @param const std::string& jsonResponse
   */
  bool Util::hasAPIResponseError(const std::string &jsonResponse) {
    Document document;
    document.Parse<rapidjson::kParseStopWhenDoneFlag>(jsonResponse.c_str());

    if(document.HasParseError()){
      fprintf(stderr, "JSON Parse Error: %s\n", GetParseError_En(document.GetParseError()));
      return true;
    }

    if(document.HasMember("errorCode")){
      fprintf(stderr, "API Error (%s): %s\n", document["errorCode"].GetString(), document["errorDescr"].GetString());
      return true;
    }

    return false;
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

  /*!
   * trim from left
   *
   * @param char* s
   * @return char*
   */
  char *Util::ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
  }

  /*!
   * trim from right
   *
   * @param char* s
   * @return char*
   */
  char *Util::rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
  }

  /*!
   * trim left and right
   *
   * @param char* s
   * @return char*
   */
  char *Util::trim(char *s) {
    return rtrim(ltrim(s));
  }

  /*!
   * Get UTC timestamp in milliseconds
   *
   * @param int t_day
   * @param unsigned int t_month
   * @param int t_year
   * @param int t_hour
   * @param int t_minute
   * @param int t_second
   * @return long long int
   */
  long long int Util::getUTCinMilliseconds(int t_day, unsigned int t_month, int t_year, int t_hour, int t_minute, int t_second) {
    auto ymd = month(t_month)/t_day/t_year;
    auto tod = make_time(hours{t_hour} + minutes{t_minute} + seconds{t_second});
    system_clock::time_point tp = sys_days(ymd) + seconds(tod);

    auto ms = duration_cast<milliseconds>(tp.time_since_epoch());

    return ms.count();
  }
}