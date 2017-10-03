//
// Created by Arne Gockeln on 03.09.17.
//

#include "Util.h"
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include "date/date.h"
#include "date/tz.h"
#include <chrono>
#include <vector>

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
   * Get UTC NOW timestamp in milliseconds
   *
   * @return long long int
   */
  long long int Util::getUTCTimestampMS() {
    return getMilliseconds( getUTCTimestamp() ).count();
  }

  /*!
   * Get UTC timestamp difference from now in milliseconds
   *
   * @param const long long int seconds
   * @return long long int
   */
  long long int Util::getUTCTimestampDifferenceMS(const long long int seconds){
    return getMilliseconds( getUTCTimestampDifference( seconds ) ).count();
  }

  /**
   * Convert utc timestamp to date string Y-m-d I:M:S p
   * @param long long t_timestamp
   * @return std::string
   */
  std::string Util::getUTCDateFromTimestamp(long long t_timestamp) {
    date::sys_seconds tp{std::chrono::seconds{t_timestamp}};
    std::string s = date::format("%Y-%m-%d %I:%M:%S %p", tp);
    return s;
  }

  /*!
   * Parse iso8601 datetime string to sys_seconds
   *
   * @param std::string& str
   * @return date::sys_seconds
   */
  date::sys_seconds Util::parseISO8601(const std::string& str){
    std::istringstream in(str);
    date::sys_seconds tp;
    in >> date::parse("%FT%TZ", tp);
    if( in.fail() ){
      in.clear();
      in.str(str);
      in >> date::parse("%FT%T%z", tp);
    }
    return tp;
  }

  /*!
   * Get utc timestamp in seconds
   *
   * @return std::chrono::seconds
   */
  std::chrono::seconds Util::getUTCTimestamp(){
    auto now = std::chrono::system_clock::now();
    auto result = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    return result;
  }

  /*!
   * Get utc timestamp with difference
   *
   * @param const long long int seconds
   * @return
   */
  std::chrono::seconds Util::getUTCTimestampDifference(const long long int seconds){
    auto now = std::chrono::system_clock::now();
    auto diff = now - std::chrono::seconds( seconds );

    // get seconds
    auto result = std::chrono::duration_cast<std::chrono::seconds>( diff.time_since_epoch() );

    return result;
  }

  /*!
   * Convert seconds to milliseconds
   *
   * @param const std::chrono::seconds seconds
   * @return std::chrono::milliseconds
   */
  std::chrono::milliseconds Util::getMilliseconds(const std::chrono::seconds seconds){
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(seconds);
    return ms;
  }

  /*!
   * Implementation of phps explode(string, delimiter)
   *
   * @param std::string const& s
   * @param char dlim
   * @return std::vector<std::string>
   */
  std::vector<std::string> Util::explode(std::string const &s, char dlim) {
    std::vector<std::string> result;
    std::istringstream iss(s);

    for(std::string token; std::getline(iss, token, dlim); ){
      result.push_back(std::move(token));
    }

    return result;
  }
}