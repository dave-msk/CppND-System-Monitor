#ifndef CPPND_SYSTEM_MONITOR_UTIL_H_
#define CPPND_SYSTEM_MONITOR_UTIL_H_

#include <fstream>
#include <vector>
#include <string>

// Classic helper function
class Util {
 public:
  static std::string ConvertToTime(long int input_seconds);
  static std::string GetProgressBar(const std::string &percent);
  static std::ifstream GetStream(const std::string &path);
  static std::vector<std::string> SplitStringByWhitespace(const std::string&);
};

#endif  // CPPND_SYSTEM_MONITOR_UTIL_H_