#include "process.h"

#include <unistd.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

void Process::setID(int pid)
{
	pid_ = pid;
}

int Process::getRam(void) const
{
	return ram_;
}

void Process::setRam(int ram)
{
	ram_ = ram;
}

// TODO: Return this process's ID
int Process::Pid() const {
	return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const 
{ 
	return LinuxParser::getProcessCPU(Process::Pid());
}

// TODO: Return the command that generated this process
string Process::Command() { 
	return LinuxParser::Command(Process::Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
	return LinuxParser::Ram(Process::Pid());
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
	return LinuxParser::User(Process::Pid());
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
	return LinuxParser::UpTime(Process::Pid())/100;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) {
  if (this->CpuUtilization() > a.CpuUtilization())
  {
  	return true;
  }
  else 
  {
  	return false;
  }
}
