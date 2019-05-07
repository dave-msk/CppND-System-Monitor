#ifndef CPPND_SYSTEM_MONITOR_PROCESS_CONTAINER_H_
#define CPPND_SYSTEM_MONITOR_PROCESS_CONTAINER_H_

#include <string>
#include <vector>

#include "process.h"

class ProcessContainer {
 public:
  ProcessContainer();
  void RefreshList();
  std::string PrintList();
  std::vector<std::vector<std::string>> GetList();

 private:
  std::vector<Process> list_;
};

#endif  // CPPND_SYSTEM_MONITOR_PROCESS_CONTAINER_H_