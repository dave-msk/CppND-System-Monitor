#ifndef CPPND_SYSTEM_MONITOR_CONSTANTS_H_
#define CPPND_SYSTEM_MONITOR_CONSTANTS_H_

#include <string>

enum class CPUStates {
  S_USER = 1,
  S_NICE,
  S_SYSTEM,
  S_IDLE,
  S_IOWAIT,
  S_IRQ,
  S_SOFTIRQ,
  S_STEAL,
  S_GUEST,
  S_GUEST_NICE
};

constexpr int GetCPUStatesValue(CPUStates state) {
  return static_cast<int>(state);
}

class Path {
 public:
  static std::string BasePath() {
    return "/proc/";
  }
  static std::string CmdPath() {
    return "/cmdline";
  }
  static std::string CpuInfoPath() {
    return "cpuinfo";
  }
  static std::string StatusPath() {
    return "/status";
  }
  static std::string StatPath() {
    return "stat";
  }
  static std::string UpTimePath() {
    return "uptime";
  }
  static std::string MemInfoPath() {
    return "meminfo";
  }
  static std::string VersionPath() {
    return "version";
  }
  static std::string PasswdPath() {
    return "/etc/passwd";
  }
  static std::string ThreadsPath() {
    return "sys/kernel/threads-max";
  }
  static std::string OsReleasePath() {
    return "/etc/os-release";
  }
};

#endif  // CPPND_SYSTEM_MONITOR_CONSTANTS_H_
