//
// Created by Arne Gockeln on 03.09.17.
//

#ifndef XTBCLIENT_UTIL_H
#define XTBCLIENT_UTIL_H

#include <rapidjson/document.h>
#include <string>
#include <chrono>
#include <date/date.h>

using namespace rapidjson;

namespace xtbclient {
  class Util {
  public:
    static bool hasDocumentParseError(Document *t_document);
    static void handleJsonResponseError(std::string* jsonData);
    static bool hasAPIResponseError(const std::string &jsonResponse);
    static void debug(std::string data){
      fprintf(stdout, "Debug: %s\n", data.data());
    }
    static void printError(const std::string& message) {
      fprintf(stderr, "Error: %s\n", message.c_str());
    }
    static void printMessage(const std::string& message){
      fprintf(stdout, "xtbclient: %s\n", message.c_str());
    }
    static void ltrim(std::string &s);
    static void rtrim(std::string &s);
    static void trim(std::string &s);
    static std::string ltrim_copy(std::string s);
    static std::string rtrim_copy(std::string s);
    static std::string trim_copy(std::string s);

    static char* ltrim(char *s);
    static char* rtrim(char *s);
    static char* trim(char *s);

    static long long int getUTCinMilliseconds(int day, unsigned int month, int year, int hour, int minute, int second);
    static long long int getUTCinMillisecondsNow();
    static long long int getUTCinMillisecondsFromNow(int days, unsigned int months, int years, int hours, int minutes, int seconds);
    static std::string getUTCDateFromTimestamp(long long t_timestamp);

    static std::vector<std::string> explode(std::string const& s, char dlim);
  };
}


#endif //XTBCLIENT_UTIL_H
