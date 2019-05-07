#include "sys_info.h"

#include <iostream>
#include <string>
#include <vector>

#include "process_parser.h"
#include "util.h"

SysInfo::SysInfo() {
  /*
  Getting initial info about system
  Initial data for individual cores is set
  System data is set
  */
  this->GetOtherCores(ProcessParser::GetNumberOfCores());
  this->SetLastCpuMeasures();
  this->SetAttributes();
  this->os_name_ = ProcessParser::GetOSName();
  this->kernel_ver_ = ProcessParser::GetSysKernelVersion();
}

void SysInfo::GetOtherCores(int _size) {
  // when number of cores is detected,
  // vectors are modified to fit incoming data
  // TODO: May use `vector(size)` instead
  this->cores_stats_ = std::vector<std::string>();
  this->cores_stats_.resize(_size);
  this->last_cpu_cores_stats_ = std::vector<std::vector<std::string>>();
  this->last_cpu_cores_stats_.resize(_size);
  this->current_cpu_cores_stats_ = std::vector<std::vector<std::string>>();
  this->current_cpu_cores_stats_.resize(_size);
  for (int i = 0; i < _size; ++i) {
    this->last_cpu_cores_stats_[i] = ProcessParser::GetSysCpuPercent(
        std::to_string(i));
  }
}

void SysInfo::SetLastCpuMeasures() {
  this->last_cpu_stats_ = ProcessParser::GetSysCpuPercent();
}

void SysInfo::SetCpuCoresStats() {
  // Getting data from files (previous data is required)
  for (int i = 0; i < this->current_cpu_cores_stats_.size(); ++i) {
    this->current_cpu_cores_stats_[i] = ProcessParser::GetSysCpuPercent(
        std::to_string(i));
  }
  for (int i = 0; i < this->current_cpu_cores_stats_.size(); ++i) {
    // after acquirement of data we are calculating every
    // core percentage of usage.
    this->cores_stats_[i] = ProcessParser::PrintCpuStats(
        this->last_cpu_cores_stats_[i], this->current_cpu_cores_stats_[i]);
  }
  this->last_cpu_cores_stats_ = this->current_cpu_cores_stats_;
}

void SysInfo::SetAttributes() {
  // Getting parsed data
  this->mem_percent_ = ProcessParser::GetSysRamPercent();
  this->up_time_ = ProcessParser::GetSysUpTime();
  this->total_proc_ = ProcessParser::GetTotalNumberOfProcesses();
  this->running_proc_ = ProcessParser::GetNumberOfRunningProcesses();
  this->threads_ = ProcessParser::GetTotalThreads();
  this->current_cpu_stats_ = ProcessParser::GetSysCpuPercent();
  this->cpu_percent_ = ProcessParser::PrintCpuStats(this->last_cpu_stats_,
                                                    this->current_cpu_stats_);
  this->last_cpu_stats_ = this->current_cpu_stats_;
  this->SetCpuCoresStats();
}

// Constructing string for every core data display
std::vector<std::string> SysInfo::GetCoresStats() const {
  std::vector<std::string> result = std::vector<std::string>();
  for (int i = 0; i < this->cores_stats_.size(); ++i) {
    std::string temp = "cpu" + std::to_string(i) + ": ";
    float check;
    if (!this->cores_stats_[i].empty())
      check = std::stof(this->cores_stats_[i]);
    if (!check || this->cores_stats_[i] == "nan") {
      return std::vector<std::string>();
    }
    temp += Util::GetProgressBar(this->cores_stats_[i]);
    result.push_back(temp);
  }
  return result;
}

std::string SysInfo::GetCpuPercent() const {
  return this->cpu_percent_;
}

std::string SysInfo::GetMemPercent() const {
  return std::to_string(this->mem_percent_);
}

long SysInfo::GetUpTime() const {
  return this->up_time_;
}

std::string SysInfo::GetKernelVersion() const {
  return this->kernel_ver_;
}

std::string SysInfo::GetTotalProc() const {
  return std::to_string(this->total_proc_);
}

std::string SysInfo::GetRunningProc() const {
  return std::to_string(this->running_proc_);
}

std::string SysInfo::GetThreads() const {
  return std::to_string(this->threads_);
}

std::string SysInfo::GetOSName() const {
  return this->os_name_;
}