#include "includes/logger.hpp"
#include <ctime>
#include <sstream>
#include <iostream>

Output::Output(std::string fileName) {
  this->file = fopen(fileName.c_str(), "w");
  if (this->file == NULL) {
    std::cerr << "Error opening log file!" << std::endl;
  }
}

Output::~Output() {
  fclose(this->file);
}

void format_num(int num, char *str) {
  sprintf(str, "%02d", num);
}

std::string append_num(std::string s, int num) {
  char buf[3];
  format_num(num, buf);
  return (s + buf);
}

void Output::Log(float temp, float ph) {
  const std::time_t t = time(0);
  std::tm *calendar_time = localtime(&t);

  char buf[3];

  std::string string = std::string("") + std::to_string(calendar_time->tm_year + 1900) + "-";
  
  string = append_num(string, calendar_time->tm_mon + 1) + "-";
  string = append_num(string, calendar_time->tm_mday) + "+";
  string = append_num(string, calendar_time->tm_hour) + ":";
  string = append_num(string, calendar_time->tm_min) + ":";
  string = append_num(string, calendar_time->tm_sec) + " ";

  string += std::to_string(temp) + " ";
  string += std::to_string(ph) + "\n";
  
  std::cout << "Logging: " << string;

  fprintf(this->file, "%s", string.c_str());
  fflush(this->file);
}