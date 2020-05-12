#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
	string line;
	string key;
	string value;
	std::ifstream filestream(kOSPath);
	if (filestream.is_open()) {
		while (std::getline(filestream, line)) {
			std::replace(line.begin(), line.end(), ' ', '_');
			std::replace(line.begin(), line.end(), '=', ' ');
			std::replace(line.begin(), line.end(), '"', ' ');
			std::istringstream linestream(line);
			while (linestream >> key >> value) {
				if (key == "PRETTY_NAME") {
					std::replace(value.begin(), value.end(), '_', ' ');
					return value;
				}
			}
		}
	}
	return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
	string os, kernel;
	string line;
	string filler;
	string kernelVersion;
	std::ifstream stream(kProcDirectory + kVersionFilename);
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> os >> filler >> kernelVersion;
	}
	return kernelVersion;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
	vector<int> pids;
	DIR* directory = opendir(kProcDirectory.c_str());
	struct dirent* file;
	while ((file = readdir(directory)) != nullptr) {
		// Is this a directory?
		if (file->d_type == DT_DIR) {
			// Is every character of the name a digit?
			string filename(file->d_name);
			if (std::all_of(filename.begin(), filename.end(), isdigit)) {
				int pid = stoi(filename);
				pids.push_back(pid);
			}
		}
	}
	closedir(directory);
	return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
	string line;
	string key;
	string value;
	float TotalMemory = 1;
	float FreeMemory = 1;
	float UsedMemory = 1;
	std::ifstream stream(kProcDirectory + kMeminfoFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key) {
				if (key == "MemTotal:") {
					linestream >> value;
					TotalMemory = std::stof(value);
				}
				if (key == "MemFree:") {
					linestream >> value;
					FreeMemory = std::stof(value);
					UsedMemory = TotalMemory - FreeMemory;
					return UsedMemory/TotalMemory;
				}
			}
		}

	}

	return 0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
	string line;
	string systemUpTime;
	long lsystemUpTime = 0;
	std::ifstream stream("/proc/uptime");
	if (stream.is_open()) {
		std::getline(stream, line);
		std::istringstream linestream(line);
		linestream >> systemUpTime;
		lsystemUpTime = stol(systemUpTime);
		return lsystemUpTime;
	}
	return lsystemUpTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
	long jiffies;
	std::string key;
	std::string line;
	std::string value;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if(stream.is_open())
	{
		std::getline(stream,line);
		std::stringstream linestream(line);
		while (linestream >> key)
		{
			if (key == "cpu")
			{
				while(linestream >> value)
				{
					jiffies += stol(value);
				}
				return jiffies;
			}
		}
	}
	return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
	string line;
	string key;
	string value;
	long activeJiffies = 0;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if (stream.is_open()) {
		std::getline(stream,line);
		std::stringstream linestream(line);
		while (linestream >> key)
		{
			if (key == "cpu")
			{
				for (int i = 0; i < 3; i++)
				{
					linestream >> value;
					activeJiffies += stol(value);
				}
				return activeJiffies;
			}
		}
	}
	return 0;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{ 
	string line;
	string key;
	string value;
	long idleJiffies = 1;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if (stream.is_open()) 
	{
		std::getline(stream,line);
		std::istringstream linestream(line);     
		while (linestream >> key)
		{
			if (key == "cpu")
			{
				for (int i = 0; i < 3; i++)
				{
					linestream >> value;
				}
				//Getting idle 
				linestream >> value;
				idleJiffies = stol(value);
				//Getting iowait
				linestream >> value;
				idleJiffies += stol(value);
				return idleJiffies;
			}
		}
	}
	return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{ 
	vector<string>  CpuUtil;
	return CpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
	string line;
	string key;
	string value;
	int totalProcesses;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key) {
				if (key == "processes") {
					linestream >> totalProcesses;
					return totalProcesses;
				}
			}
		}
	}
	return 0;
}

float LinuxParser::getProcessCPU(int pid)
{
	std::string line;
	std::string procStatPath = "/proc/" + std::to_string(pid) + "/stat";
	std::string ignore;
	long utime;
	long stime;
	long cutime;
	long cstime;
	long starttime;
	std::ifstream procStatFile;
	procStatFile.open(procStatPath);
	if (procStatFile.is_open())
	{
		std::getline(procStatFile,line);
		std::istringstream linestream(line);
		for (int i = 0; i < 13; i++)
		{
			linestream >> ignore;
		}
		linestream >> utime >> stime >> cutime >> cstime;
		for (int i = 0; i < 4; i++)
		{
			linestream >> ignore;
		}
		linestream >> starttime;
		long seconds = LinuxParser::UpTime(pid) - (starttime)/100;
		return  (float) (((utime + stime + cutime + cstime)/100)/seconds);

	}
	return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
	string line;
	string key;
	string value;
	int runningProcesses;
	std::ifstream stream(kProcDirectory + kStatFilename);
	if (stream.is_open()) {
		while (std::getline(stream, line)) {
			std::istringstream linestream(line);
			while (linestream >> key) {
				if (key == "procs_running") {
					linestream >> runningProcesses;
					return runningProcesses;
				}
			}
		}
	}
	return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{
	std::string key;
	std::string line;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + "/cmdline");
	if(stream.is_open())
	{
		while(std::getline(stream,line))
		{
			std::istringstream linestream(line);
			linestream >> key;
			return key;
		}
	}
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
	std::string key;
	std::string line;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + "/status");
	if(stream.is_open())
	{
		while(std::getline(stream,line))
		{
			std::istringstream linestream(line);
			linestream >> key;
			if (key == "VmSize:")
			{
				linestream >> key;
				return key;
			}
		}
	}
	return "olleH";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) 
{
	std::string line;
	std::string key;
	std::ifstream stream(kProcDirectory + std::to_string(pid) + "/status");
	if (stream.is_open())
	{
		while(std::getline(stream,line))
		{
			std::istringstream linestream(line);
			while (linestream >> key)
			{
				if (key == "Uid:")
				{
					linestream >> key;
					return key;
				}
			}
		}
	}

	return "h"; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{ 
	std::string line;
	std::string key;
	std::string user;
	std::string ignore;
	std::string id;
	std::string userID = LinuxParser::Uid(pid);
	std::ifstream userFileStream;
	userFileStream.open("/etc/passwd");
	if (userFileStream.is_open())
	{
		while(std::getline(userFileStream,line))
		{
			std::replace(line.begin(),line.end(),':',' ');
			std::istringstream  linestream(line);	
			linestream >> user >> ignore >> id;	
			if (id == userID)
			{
				return user;
			}
		}
	}

	return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {	
	std::string line;
	std::string key;
	std::string ignore;
	std::ifstream procUptimeFile;
	std::string procUptimePath = "/proc/" + std::to_string(pid) + "/stat";
	procUptimeFile.open(procUptimePath);
	if (procUptimeFile.is_open())
	{
		std::getline(procUptimeFile,line);
		std::istringstream linestream(line);
		for (int i = 0; i < 21; i++)
		{
		linestream >> ignore;
		}
		linestream >> key;
		long value = std::stol(key);
		return value;
	}
	return 1;
}
