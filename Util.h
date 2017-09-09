//
// Created by Arne Gockeln on 03.09.17.
//

#ifndef XTBCLIENT_UTIL_H
#define XTBCLIENT_UTIL_H

#include <rapidjson/document.h>
#include <string>

using namespace rapidjson;

namespace xtbclient {
  class Util {
  public:
    static bool hasDocumentParseError(Document *t_document);
    static void handleJsonResponseError(std::string* jsonData);
    static void debug(std::string data){
      fprintf(stdout, "Debug: %s\n", data.data());
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
  };
}


#endif //XTBCLIENT_UTIL_H
