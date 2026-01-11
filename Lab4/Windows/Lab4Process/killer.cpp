#include <iostream>
#include "windows.h"
#include <tlhelp32.h>
#include <sstream>

void killByName(const char* str) {
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE) {
		std::cerr << "CreateToolhelp32Snapshot failed." << std::endl;
		return;
	}

	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL pRes = Process32First(hSnapShot, &pEntry);

	while (pRes) {
		const size_t size = strlen(str) + 1;
		wchar_t* temp = new wchar_t[size];
		size_t outSize;
		errno_t err = mbstowcs_s(&outSize, temp, size, str, size - 1);

		if (err != 0) {
			std::wcerr << L"Error converting string: " << str << std::endl;
			delete[] temp;
			continue;
		}

		if (_wcsicmp(pEntry.szExeFile, temp) == 0) {
			HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pEntry.th32ProcessID);
			if (TerminateProcess(processHandle, -1)) {
				DWORD waitResult = WaitForSingleObject(processHandle, 5000);
				if (waitResult != 0)	std::cerr << "TerminateProcess failed" << "\n";

			}
			else std::cerr << "TerminateProcess failed"  << std::endl;
			CloseHandle(processHandle);
		}
		delete[] temp;
		pRes = Process32Next(hSnapShot, &pEntry);
	}
}

void killFromEnv() {
	DWORD size = GetEnvironmentVariableA("PROC_TO_KILL", nullptr, 0);
	std::string envVar(size - 1, '\0');
	DWORD lenEnvVar = GetEnvironmentVariableA("PROC_TO_KILL", &envVar[0], size);
	if (lenEnvVar == 0) return;
	
	std::stringstream ss(envVar);

	std::string temp;

	while (getline(ss, temp, '|')) {
		const char* processName = temp.c_str();
		killByName(processName);
	}
}

void killById(const char* str) {
	DWORD processId = 0;
	sscanf_s(str, "%u", &processId);
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (TerminateProcess(processHandle, -1)) {
		DWORD waitResult = WaitForSingleObject(processHandle, 5000);
		if(waitResult != 0) std::cerr << "TerminateProcess failed" << std::endl;
	}
	else std::cerr << "TerminateProcess failed" << std::endl;
	CloseHandle(processHandle);
}


int main(int argc, char* argv[])
	
{	
  	int exitCode = -1;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--id") == 0) {
			if (i + 1 < argc) {
				i++;
				killById(argv[i]);
			}
		}
		else if (strcmp(argv[i], "--name") == 0) { 
			if (i + 1 < argc) {
				i++;
				killByName(argv[i]);
			}
		}
		
		else {
			std::cout<<"Warning: Unknown argument '" << argv[i]<< "\' ignored."<<std::endl;
		}
	}
	killFromEnv();
}