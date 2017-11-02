/*!
 *  Copyright(c)2017, Arne Gockeln
 *  http://www.arnegockeln.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef XTBCLIENT_UTIL_H
#define XTBCLIENT_UTIL_H

#include <rapidjson/document.h>
#include <string>
#include <chrono>
#include <date/date.h>
#include <vector>

using namespace rapidjson;

namespace xtbclient {
  class Util {
  public:
    static bool hasDocumentParseError(Document *t_document);
    static void handleJsonResponseError(std::string* jsonData);
    static bool hasAPIResponseError(const std::string &jsonResponse);
    static void ltrim(std::string &s);
    static void rtrim(std::string &s);
    static void trim(std::string &s);
    static std::string ltrim_copy(std::string s);
    static std::string rtrim_copy(std::string s);
    static std::string trim_copy(std::string s);

    static char* ltrim(char *s);
    static char* rtrim(char *s);
    static char* trim(char *s);

    static std::string getUTCDateFromTimestamp(long long t_timestamp);
    static date::sys_seconds parseISO8601(const std::string& str);
    static std::chrono::seconds getUTCTimestamp();
    static std::chrono::seconds getUTCTimestampDifference(const long long int seconds);
    static long long int getUTCTimestampMS();
    static long long int getUTCTimestampDifferenceMS(const long long int seconds);
    static std::chrono::milliseconds getMilliseconds(const std::chrono::seconds seconds);

    static std::vector<std::string> explode(std::string const& s, char dlim);
  };
}


#endif //XTBCLIENT_UTIL_H
