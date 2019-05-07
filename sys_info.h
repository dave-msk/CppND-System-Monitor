#ifndef CPPND_SYSTEM_MONITOR_SYS_INFO_H_
#define CPPND_SYSTEM_MONITOR_SYS_INFO_H_

#include <string>
#include <vector>

#include "process_parser.h"

class SysInfo {
 public:
  SysInfo();
  void SetAttributes();
  void SetLastCpuMeasures();
  std::string GetMemPercent() const;
  long GetUpTime() const;
  std::string GetThreads() const;
  std::string GetTotalProc() const;
  std::string GetRunningProc() const;
  std::string GetKernelVersion() const;
  std::string GetOSName() const;
  std::string GetCpuPercent() const;
  void GetOtherCores(int _size);
  void SetCpuCoresStats();
  std::vector<std::string> GetCoresStats() const;

 private:
  std::vector<std::string> last_cpu_stats_;
  std::vector<std::string> current_cpu_stats_;
  std::vector<std::string> cores_stats_;
  std::vector<std::vector<std::string>> last_cpu_cores_stats_;
  std::vector<std::vector<std::string>> current_cpu_cores_stats_;
  std::string cpu_percent_;
  float mem_percent_;
  std::string os_name_;
  std::string kernel_ver_;
  long up_time_;
  int total_proc_;
  int running_proc_;
  int threads_;
};

#endif  // CPPND_SYSTEM_MONITOR_SYS_INFO_H_
