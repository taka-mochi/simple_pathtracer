#pragma once

#include <string>
#include <vector>
#include <sstream>

class Utils {
public:
  static std::vector<std::string> split(const std::string &str, char delim) {
    std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
    while (std::getline(iss, tmp, delim)) res.push_back(tmp);
    return res;
  }
};
