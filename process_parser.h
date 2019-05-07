#ifndef CPPND_SYSTEM_MONITOR_PROCESS_PARSER_H_
#define CPPND_SYSTEM_MONITOR_PROCESS_PARSER_H_

#include <algorithm>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <ctime>
#include <unistd.h>

#include "constants.h"


class ProcessParser {
 public:
  static std::string GetCmd(const std::string &pid);
  static std::vector<std::string> GetPidList();
  static std::string GetVmSize(const std::string &pid);
  static std::string GetCpuPercent(const std::string &pid);
  static long int GetSysUpTime();
  static std::string GetProcUpTime(const std::string &pid);
  static std::string GetProcUser(const std::string &pid);
  static std::vector<std::string> GetSysCpuPercent(
      std::string core_number="");
  static float GetSysRamPercent();
  static std::string GetSysKernelVersion();
  static int GetNumberOfCores();
  static int GetTotalThreads();
  static int GetTotalNumberOfProcesses();
  static int GetNumberOfRunningProcesses();
  static std::string GetOSName();
  static std::string PrintCpuStats(
      const std::vector<std::string> &values1,
      const std::vector<std::string> &values2);
  static bool IsPidExisting(const std::string &pid);
 private:
  std::ifstream stream;
};

#endif  // CPPND_SYSTEM_MONITOR_PROCESS_PARSER_H_

// TODO: Define all of the above functions
