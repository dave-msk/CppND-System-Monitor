#include "process.h"

#include <string>

#include "process_parser.h"


void Process::SetPid(int pid) {
  this->pid_ = std::to_string(pid);
}

std::string Process::GetPid() const {
  return this->pid_;
}

std::string Process::GetUser() const {
  return this->user_;
}

std::string Process::GetCmd() const {
  return this->cmd_;
}

int Process::GetCpu() const {
  return std::stoi(this->cpu_);
}

int Process::GetMem() const {
  return std::stoi(this->mem_);
}

std::string Process::GetUpTime() const {
  return this->up_time_;
}

std::string Process::GetProcess() {
  if (!ProcessParser::IsPidExisting(this->pid_))
    return "";
  this->mem_ = ProcessParser::GetVmSize(this->pid_);
  this->up_time_ = ProcessParser::GetProcUpTime(this->pid_);
  this->cpu_ = ProcessParser::GetCpuPercent(this->pid_);

  return (this->pid_ + "   "   + this->user_
                     + "   "   + this->mem_.substr(0, 5)
                     + "     " + this->cpu_.substr(0, 5)
                     + "     " + this->up_time_.substr(0, 5)
                     + "    "  + this->cmd_.substr(0, 30)
                     + "...");
}
