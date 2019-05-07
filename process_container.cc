#include "process_container.h"

#include <string>
#include <vector>

#include "process_parser.h"

ProcessContainer::ProcessContainer() {
  this->RefreshList();
}

void ProcessContainer::RefreshList() {
  std::vector<std::string> pid_list = ProcessParser::GetPidList();
  this->list_.clear();
  for (int i = 0; i < pid_list.size(); ++i) {
    this->list_.push_back(pid_list[i]);
  }
}

std::string ProcessContainer::PrintList() {
  std::string result = "";
  for (int i = 0; i < this->list_.size(); ++i) {
    result += this->list_[i].GetProcess();
  }
  return result;
}

std::vector<std::vector<std::string>> ProcessContainer::GetList() {
  std::vector<std::vector<std::string>> values;
  std::vector<std::string> stringified_list;
  for (int i = 0; i < this->list_.size(); ++i) {
    stringified_list.push_back(this->list_[i].GetProcess());
  }
  int last_index = 0;
  for (int i = 0; i < stringified_list.size(); ++i) {
    if (i > 0 && i % 10 == 0) {
      std::vector<std::string> sub(&stringified_list[i-10],
                                   &stringified_list[i]);
      values.push_back(sub);
      last_index = i;
    }
    if (i == (this->list_.size() - 1) && (i - last_index) < 10) {
      std::vector<std::string> sub(&stringified_list[last_index],
                                   &stringified_list[i+1]);  // list.end() ?
      values.push_back(sub);
    }
  }
  return values;
}