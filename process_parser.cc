#include "process_parser.h"

#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <unistd.h>

#include "constants.h"
#include "util.h"


static float GetSysActiveCpuTime(const std::vector<std::string> values) {
  return (std::stof(values[GetCPUStatesValue(CPUStates::S_USER)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_NICE)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_SYSTEM)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_IRQ)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_SOFTIRQ)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_STEAL)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_GUEST)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_GUEST_NICE)]));
}

static float GetSysIdleCpuTime(const std::vector<std::string> &values) {
  return (std::stof(values[GetCPUStatesValue(CPUStates::S_IDLE)]) +
          std::stof(values[GetCPUStatesValue(CPUStates::S_IOWAIT)]));
}


std::string ProcessParser::GetCmd(const std::string &pid) {
  std::string line;
  auto stream = Util::GetStream(Path::BasePath() + pid + Path::CmdPath());
  std::getline(stream, line);
  return line;
}

std::vector<std::string> ProcessParser::GetPidList() {
  DIR *dir;
  std::vector<std::string> container;
  if (!(dir = opendir(Path::BasePath().c_str())))
    throw std::runtime_error(std::strerror(errno));
  
  while (dirent *dirp = readdir(dir)) {
    // Check if it's a directory
    if (dirp->d_type != DT_DIR) continue;
    // Check if every character in the name is a digit
    if (std::all_of(dirp->d_name,
                    dirp->d_name + std::strlen(dirp->d_name),
                    [](char c){ return std::isdigit(c); })) {
          container.push_back(dirp->d_name);
    }
  }
  // Validating process of directory closing
  if (closedir(dir))
    throw std::runtime_error(std::strerror(errno));
  return container;
}

std::string ProcessParser::GetVmSize(const std::string &pid) {
  std::string line;
  // Declaring search attribute for file
  const std::string name = "VmData";
  float result = 0;
  // Opening stream for specific file
  auto stream = Util::GetStream(Path::BasePath() + pid + Path::StatusPath());
  while (std::getline(stream, line)) {
    // Searching line by line
    if (line.compare(0, name.size(), name) == 0) {
      // Slicing string line on ws for values using sstream
      auto values = Util::SplitStringByWhitespace(line);
      // Conversion kB -> MB
      result = std::stof(values[1]) / float(1024);
      break;
    }
  }
  return std::to_string(result);
}

std::string ProcessParser::GetCpuPercent(const std::string &pid) {
  auto stream = Util::GetStream(
      Path::BasePath() + pid + "/" + Path::StatPath());
  std::string line;
  std::getline(stream, line);
  auto values = Util::SplitStringByWhitespace(line);
  const std::size_t orig_comm = 1;
  std::size_t comm = 1;

  for (std::size_t i = 1; i < values.size(); ++i) {
    if (values[i].back() == ')') {
      comm = i;
      break;
    }
  }
  
  const std::size_t offset = comm - orig_comm;
  const float utime = std::stof(GetProcUpTime(pid));
  const float stime = std::stof(values[14 + offset]);   // stime  (14)
  const float cutime = std::stof(values[15 + offset]);  // cutime (15)
  const float cstime = std::stof(values[16 + offset]);  // cstime (16)

  const float freq = sysconf(_SC_CLK_TCK);
  const float proc_uptime_s = std::stof(GetProcUpTime(pid));
  const float total_s = utime + (stime + cutime + cstime) / freq;
  const float result = 100.0 * total_s / proc_uptime_s;
  return std::to_string(result);
}

long int ProcessParser::GetSysUpTime() {
  auto stream = Util::GetStream(Path::BasePath() + Path::UpTimePath());
  std::string line;
  std::getline(stream, line);
  auto values = Util::SplitStringByWhitespace(line);
  return std::stol(values[0]);
}

std::string ProcessParser::GetProcUpTime(const std::string &pid) {
  std::string line;
  auto stream = Util::GetStream(
      Path::BasePath() + pid + "/" + Path::StatPath());
  std::getline(stream, line);
  auto values = Util::SplitStringByWhitespace(line);

  const std::size_t orig_comm = 1;
  std::size_t comm = 1;
  for (std::size_t i = 1; i < values.size(); ++i) {
    if (values[i].back() == ')') {
      comm = i;
      break;
    }
  }

  const std::size_t offset = comm - orig_comm;
  const float utime = std::stof(values[13 + offset]);  // utime (13)
  const float freq = sysconf(_SC_CLK_TCK);
  const float result = utime / freq;
  return std::to_string(result);
}

std::string ProcessParser::GetProcUser(const std::string &pid) {
  std::string line;
  const std::string name = "Uid:";
  std::string result = "";
  auto stream = Util::GetStream(Path::BasePath() + pid + Path::StatusPath());
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      result = values[1];
      break;
    }
  }
  stream = Util::GetStream(Path::PasswdPath());
  const std::string key = "x:" + result;
  while (std::getline(stream, line)) {
    if (line.find(key) != std::string::npos) {
      result = line.substr(0, line.find(":"));
      break;
    }
  }
  return result;
}

int ProcessParser::GetNumberOfCores() {
  const std::string name = "cpu cores";
  std::string line;
  auto stream = Util::GetStream(Path::BasePath() + Path::CpuInfoPath());
  int cores = 0;

  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      cores = std::stoi(values.back());
      break;
    }
  }
  return cores;
}

std::vector<std::string> ProcessParser::GetSysCpuPercent(
    std::string core_number) {
  const std::string name = "cpu" + core_number;
  std::string line;
  auto stream = Util::GetStream(Path::BasePath() + Path::StatPath());
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      return Util::SplitStringByWhitespace(line);
    }
  }
  return std::vector<std::string>();
}

float ProcessParser::GetSysRamPercent() {
  const std::string ma_name = "MemAvailable:";
  const std::string mf_name = "MemFree:";
  const std::string buf_name = "Buffers:";
  float total_mem = -2;
  float free_mem = -2;
  float buffer = -2;

  std::string line;
  auto stream = Util::GetStream(Path::BasePath() + Path::MemInfoPath());
  while (std::getline(stream, line)) {
    if (total_mem > -1 && free_mem > -1 && buffer > -1) break;
    if (total_mem < 0 && line.compare(0, ma_name.size(), ma_name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      total_mem = std::stof(values[1]);
    } else if (free_mem < 0 && line.compare(0, mf_name.size(), mf_name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      free_mem = std::stof(values[1]);
    } else if (buffer < 0 && line.compare(0, buf_name.size(), buf_name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      buffer = std::stof(values[1]);
    }
  }
  return 100.0 * (1 - free_mem / (total_mem - buffer));
}

std::string ProcessParser::GetSysKernelVersion() {
  auto stream = Util::GetStream(Path::BasePath() + Path::VersionPath());
  std::string line;
  std::getline(stream, line);
  auto values = Util::SplitStringByWhitespace(line);
  return values[2];
}

int ProcessParser::GetTotalThreads() {
  std::string line;
  auto stream = Util::GetStream(Path::BasePath() + Path::ThreadsPath());
  std::getline(stream, line);
  return std::stoi(line);
}

int ProcessParser::GetTotalNumberOfProcesses() {
  return ProcessParser::GetPidList().size();
}

int ProcessParser::GetNumberOfRunningProcesses() {
  const std::string name = "procs_running";
  std::string line;
  int result = 0;
  auto stream = Util::GetStream(Path::BasePath() + Path::StatPath());
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      auto values = Util::SplitStringByWhitespace(line);
      result = std::stoi(values[1]);
    }
  }
  return result;
}

std::string ProcessParser::GetOSName() {
  const std::string name = "PRETTY_NAME";
  std::string line;
  std::string result = "";
  auto stream = Util::GetStream(Path::OsReleasePath());
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      const int start = line.find("=") + 2;
      const int len = line.size() - start - 1;
      result = line.substr(start, len);
      break;
    }
  }
  return result;
}

std::string ProcessParser::PrintCpuStats(
    const std::vector<std::string> &values1,
    const std::vector<std::string> &values2) {
  const float active_time = (GetSysActiveCpuTime(values2) - 
                             GetSysActiveCpuTime(values1));
  const float idle_time = (GetSysIdleCpuTime(values2) - 
                           GetSysIdleCpuTime(values1));
  const float result = 100.0 * (active_time / (active_time + idle_time));
  return std::to_string(result);
}

bool ProcessParser::IsPidExisting(const std::string &pid) {
  const std::vector<std::string> pid_list = ProcessParser::GetPidList();
  return std::find(pid_list.begin(), pid_list.end(), pid) != pid_list.end();
}
