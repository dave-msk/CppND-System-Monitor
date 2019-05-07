#ifndef CPPND_SYSTEM_MONITOR_PROCESS_H_
#define CPPND_SYSTEM_MONITOR_PROCESS_H_

#include <string>

#include "process_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(std::string pid) 
      : pid_(pid),
        user_(ProcessParser::GetProcUser(pid)),
        cmd_(ProcessParser::GetCmd(pid)),
        cpu_(ProcessParser::GetCpuPercent(pid)),
        mem_(ProcessParser::GetVmSize(pid)),
        up_time_(ProcessParser::GetProcUpTime(pid)) {}
  void SetPid(int pid);
  std::string GetPid() const;
  std::string GetUser() const;
  std::string GetCmd() const;
  int GetCpu() const;
  int GetMem() const;
  std::string GetUpTime() const;
  std::string GetProcess();

 private:
  std::string pid_;
  std::string user_;
  std::string cmd_;
  std::string cpu_;
  std::string mem_;
  std::string up_time_;
};

#endif  // PROCESS_H_
