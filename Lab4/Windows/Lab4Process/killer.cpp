#include <iostream>
#include "windows.h"
#include <tlhelp32.h>
#include <sstream>
#include <vector>
#include <unordered_set>

std::wstring ToLower(std::wstring str) {
	for (auto& c : str) c = towlower(c);
	return str;
}


void killById(DWORD processId) {

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (TerminateProcess(processHandle, -1)) {
		DWORD waitResult = WaitForSingleObject(processHandle, 5000);
		if (waitResult != 0) std::cerr << "TerminateProcess failed" << std::endl;
	}
	else std::cerr << "TerminateProcess failed" << std::endl;
	CloseHandle(processHandle);
}

void killByName(const std::vector<std::string>& namesToKill) {
	std::unordered_set<std::wstring> targets;
	for (const auto& name : namesToKill) {
		std::wstring wName(name.begin(), name.end()); 
		targets.insert(ToLower(wName));
	}

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		std::cerr << "CreateToolhelp32Snapshot failed." << std::endl;
		return;
	}

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL pRes = Process32First(hSnapShot, &pEntry);

	while (pRes) {
		std::wstring currentProc = ToLower(pEntry.szExeFile);

		if (targets.count(currentProc)) {
			killById((DWORD)pEntry.th32ProcessID);
			targets.erase(currentProc);
		}
		pRes = Process32Next(hSnapShot, &pEntry);
	}
	for (const auto& target : targets) {
		std::string procName(target.begin(), target.end());
		std::cout << "TerminateProcess failed" << std::endl;
	}
	CloseHandle(hSnapShot);
}

void killFromEnv(std::vector < std::string >& namesToKill) {
	DWORD size = GetEnvironmentVariableA("PROC_TO_KILL", nullptr, 0);
	std::string envVar(size - 1, '\0');
	DWORD lenEnvVar = GetEnvironmentVariableA("PROC_TO_KILL", &envVar[0], size);
	if (lenEnvVar == 0) return;
	
	std::stringstream ss(envVar);

	std::string temp;

	while (getline(ss, temp, '|')) {
		namesToKill.push_back(temp);
	}
}



int main(int argc, char* argv[])
	
{	
	std::vector < std::string > namesToKill;
  	int exitCode = -1;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--id") == 0) {
			if (i + 1 < argc) {
				i++;
				DWORD processId = 0;
				sscanf_s(argv[i], "%u", &processId);
				killById(processId);
			}
		}
		else if (strcmp(argv[i], "--name") == 0) { 
			if (i + 1 < argc) {
				i++;
				namesToKill.push_back(argv[i]);
			}
		}
		
		else {
			std::cout<<"Warning: Unknown argument '" << argv[i]<< "\' ignored."<<std::endl;
		}
	}
	killFromEnv(namesToKill);
	
	if (!namesToKill.empty()) {
		killByName(namesToKill);
	}



}