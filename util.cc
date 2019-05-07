#include "util.h"

#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

std::string Util::ConvertToTime(long int input_seconds) {
  long minutes = input_seconds / 60;
  long hours = minutes / 60;
  long seconds = int(input_seconds % 60);
  minutes = int(minutes % 60);
  std::string result = std::to_string(hours) + ":" +
                       std::to_string(minutes) + ":" +
                       std::to_string(seconds);
  return result;
}

// Constructing string for given percentage
// 50 bars is uniformly streched 0 - 100 %
// meaning: every 2% is one bar (|)
std::string Util::GetProgressBar(const std::string &percent) {

  std::string result = "0%% ";
  int _size = 50;
  int boundaries;
  try {
    boundaries = (std::stof(percent) / 100) * _size;
  } catch (...) {
    boundaries = 0;
  }

  for (int i = 0; i < _size; ++i) {
    if (i <= boundaries) result += "|";
    else result += " ";
  }

  result += " " + percent.substr(0, 5) + " /100%%";
  return result;
}

// wrapper for creating streams
std::ifstream Util::GetStream(const std::string &path) {
  std::ifstream stream(path);
  if (!stream || !stream.is_open()) {
    stream.close();
    throw std::runtime_error("Non - existing PID");
  }
  return stream;
}

std::vector<std::string> Util::SplitStringByWhitespace(const std::string &s) {
  std::istringstream buf(s);
  std::istream_iterator<std::string> begin(buf), end;
  return std::vector<std::string>(begin, end);
}