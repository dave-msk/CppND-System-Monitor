#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "process_container.h"
#include "util.h"
#include "sys_info.h"

char* GetCString(std::string str) {
  char *cstr = new char[str.length() + 1];
  std::strcpy(cstr, str.c_str());
  return cstr;
}

void WriteSysInfoToConsole(SysInfo sys, WINDOW* sys_win) {
  sys.SetAttributes();

  mvwprintw(sys_win, 2, 2, GetCString(( "OS: " + sys.GetOSName())));
  mvwprintw(sys_win, 3, 2, 
            GetCString(( "Kernel version: " + sys.GetKernelVersion())));
  mvwprintw(sys_win, 4, 2, GetCString( "CPU: "));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, GetCString(Util::GetProgressBar(sys.GetCpuPercent())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 5, 2, GetCString(( "Other cores: ")));
  wattron(sys_win, COLOR_PAIR(1));
  std::vector<std::string> val = sys.GetCoresStats();
  for (int i = 0; i < val.size(); ++i) {
    mvwprintw(sys_win, (6 + i), 2, GetCString(val[i]));
  }
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 10, 2, GetCString(( "Memory: ")));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, GetCString(Util::GetProgressBar(sys.GetMemPercent())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 11, 2,
            GetCString(( "Total Processes: " + sys.GetTotalProc())));
  mvwprintw(sys_win, 12, 2,
            GetCString(( "Running Processes:" + sys.GetRunningProc())));
  mvwprintw(sys_win, 13, 2,
            GetCString(( "Up Time: " + Util::ConvertToTime(sys.GetUpTime()))));
  wrefresh(sys_win);
}

void GetProcessListToConsole(std::vector<std::string> processes, WINDOW* win) {

  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 1, 2, "PID:");
  mvwprintw(win, 1, 9, "User:");
  mvwprintw(win, 1, 16, "CPU[%%]:");
  mvwprintw(win, 1, 26, "RAM[MB]:");
  mvwprintw(win, 1, 35, "Uptime:");
  mvwprintw(win, 1, 44, "CMD:");
  wattroff(win, COLOR_PAIR(2));
  for (int i = 0; i < processes.size(); ++i) {
    mvwprintw(win, 2+i, 2, GetCString(processes[i]));
  }
}

void PrintMain(SysInfo sys, ProcessContainer procs) {
  initscr();        /* Start curses mode */
  noecho();         // not printing input values
  cbreak();         // Terminating on classic ctrl + c
  start_color();    // Enabling color change of text
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);  // Getting size of window measured in line and
                                 // column(column one char length)
  WINDOW *sys_win = newwin(17, xMax-1, 0, 0);
  WINDOW *proc_win = newwin(15, xMax-1, 18, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  int counter = 0;
  while (1) {
    box(sys_win, 0, 0);
    box(proc_win, 0, 0);
    procs.RefreshList();
    std::vector<std::vector<std::string>> processes = procs.GetList();
    WriteSysInfoToConsole(sys, sys_win);
    GetProcessListToConsole(processes[counter], proc_win);
    wrefresh(sys_win);
    wrefresh(proc_win);
    refresh();
    sleep(1);
    if (counter == processes.size() - 1) {
      counter = 0;
    } else {
      ++counter;
    }
  }
  endwin();
}

int main(int argc, char *argv[]) {
  // Object which contains list of current processes,
  // Container for Process Class
  ProcessContainer procs;
  // Object which containts relevant methods and
  // attributes regarding system details
  SysInfo sys;
  // std::string s = WriteToConsole(sys);
  PrintMain(sys, procs);
  return 0;
}