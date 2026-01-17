#include <windows.h>
#include <tlhelp32.h>
#include <sstream>
#include <iostream>
#include <regex>
#include <thread>
#include <chrono>
#include <unordered_set>

std::wstring ToLower(std::wstring str) {
    for (auto& c : str) c = towlower(c);
    return str;
}

bool checkName(std::string& str) {
    std::regex pattern("([^\\>\\<\\:\\\"\\/\\\\\\|\\?\\*]+\\|)*([^\\>\\<\\:\\\"\\/\\\\\\|\\?\\*])+");
    return std::regex_match(str, pattern) || str == "";
}

bool checkPrId(std::string& str) {
    std::regex pattern("(\\d+\\|)*\\d+");
    return std::regex_match(str, pattern) || str == "";
}

bool checkPrExName(std::vector<std::pair<DWORD, std::string>>& vec, const std::vector <std::string>& namesToSearch) {
    if (namesToSearch.empty()) return 0;
    std::unordered_set<std::wstring> targets;
    for (const auto& name : namesToSearch) {
        std::wstring wName(name.begin(), name.end());
        targets.insert(ToLower(wName));
    }

    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof(pEntry);
    BOOL pRes = Process32First(hSnapShot, &pEntry);

    while (pRes) {
        std::wstring currentProc = ToLower(pEntry.szExeFile);


        if (targets.count(currentProc)) {
            std::string procName(currentProc.begin(), currentProc.end());
            vec.push_back(std::make_pair(pEntry.th32ProcessID, procName));
            std::cout << "process with Name = " << procName << " exists" << std::endl;
            targets.erase(currentProc);
        }
        pRes = Process32Next(hSnapShot, &pEntry);
    }
    for (const auto& target : targets) {
        std::string procName(target.begin(), target.end());
        std::cout << "process with Name = " << procName << " doesn't initially exist" << std::endl;
    }
    CloseHandle(hSnapShot);
    return 0;
}

bool byId(DWORD processId) {
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
    if (processHandle == NULL) return 0;
    DWORD exitCode;

    BOOL result = GetExitCodeProcess(processHandle, &exitCode);
    bool isRunning = (result && exitCode == STILL_ACTIVE);
    CloseHandle(processHandle);

    return isRunning;
}

bool checkPrExId(std::string& str, std::vector<DWORD>& vec) {
    DWORD processId = 0;
    sscanf_s(str.c_str(), "%u", &processId);
    if (!byId(processId)) return 0;
    vec.push_back(processId);
    return 1;
}


std::string parseAndCheckId(std::string& str, std::vector<DWORD>& vec) {
    std::stringstream ss(str);
    std::string curWord;


    std::string res = "";
    while (std::getline(ss, curWord, '|')) {
        if (checkPrExId(curWord, vec)) {
            std::cout << "process with ID = " << curWord << " exists" << std::endl;
        }
        else std::cout << "process with ID = " << curWord << " doesn't initially exist" << std::endl;
        res += "--id " + curWord + " ";

    }
    return res;
}

int createCommandId(std::string& str) {
    std::cout << "\n-------Enter process id to kill-------" << std::endl;
    std::getline(std::cin, str);
    if (!checkPrId(str)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    return 0;
}

std::string parseName(std::string& str, std::vector <std::string>& vec) {

    std::stringstream ss(str);
    std::string curWord;
    std::string res = "";
    while (std::getline(ss, curWord, '|')) {
        res += "--name \"" + curWord + "\" ";
        vec.push_back(curWord);
    }
    return res;
}

int createCommandName(std::string& str) {
    std::cout << "\n-------Enter process name to kill-------" << std::endl;
    std::getline(std::cin, str);
    if (!checkName(str)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    return 0;
}

bool setEnvVar(std::string& envVar) {
    std::cout << "Enter Env Var PROC_TO_KILL:" << std::endl;
    std::getline(std::cin, envVar);

    if (!checkName(envVar)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }

    BOOL bEv = SetEnvironmentVariableA("PROC_TO_KILL", (LPSTR)envVar.c_str());
    if (!bEv)
    {
        std::cout << "Unable to Set Environment Variable PROC_TO_KILL" << std::endl;
        std::cout << "Press any Key to quit: " << std::endl;
        return 1;
    }
    return 0;

}

void showAfterKill(std::vector < std::pair < DWORD, std::string>>& vecForName, std::vector<DWORD>& vecForId) {
    for (auto elem : vecForName) {
        bool res = byId(elem.first);
        if (res) std::cout << "process with Name = " << elem.second << " exists" << std::endl;
        else std::cout << "process with name = " << elem.second << " doesn't exist" << std::endl;
    }

    for (auto elem : vecForId) {
        bool res = byId(elem);
        if (res) std::cout << "process with ID = " << elem << " exists" << std::endl;
        else std::cout << "process with ID = " << elem << " doesn't exist" << std::endl;
    }
}


int main() {
    std::cout << "### Before Killer" << std::endl;
    std::string name = "", id = "", envVar = "";
    std::vector <std::pair<DWORD, std::string>> vecForName;
    std::vector<DWORD> vecForId;
    std::vector<std::string> names;


    if (setEnvVar(envVar)) return 1;
    envVar = parseName(envVar, names);

    if (createCommandName(name)) return 1;
    name = parseName(name, names);

    if (createCommandId(id)) return 1;
    id = parseAndCheckId(id, vecForId);

    checkPrExName(vecForName, names);

    std::string commandLine = "killer.exe " + id + " " + name;

    SECURITY_ATTRIBUTES saChProcess = { sizeof(SECURITY_ATTRIBUTES),NULL,FALSE };
    SECURITY_ATTRIBUTES saChPrimaryThread = { sizeof(SECURITY_ATTRIBUTES),NULL,FALSE };

    DWORD dwFlags = BELOW_NORMAL_PRIORITY_CLASS;

    std::cout << "\n\n### Killer Ouput:" << std::endl;

    PROCESS_INFORMATION pi;
    STARTUPINFOA si = { sizeof(STARTUPINFOA) };

    BOOL fSuccess = CreateProcessA(
        NULL,
        (LPSTR)commandLine.c_str(),
        NULL,
        NULL,
        TRUE,
        dwFlags,
        NULL,
        NULL,
        &si,
        &pi);
    if (!fSuccess)
    {
        printf("CreateProcess failed (%d)\n", GetLastError());
        printf("\nPress any Key to quit:\n");
        std::cin.get();
        return 1;
    }
    DWORD dwWait = WaitForSingleObject(pi.hProcess, INFINITE);

    if (dwWait == WAIT_TIMEOUT | dwWait == WAIT_FAILED)
    {
        std::cout << "User process terminates (kills) Killer process as it is not completed in the timeout.\n";
        std::cout << "\nPress the Key Enter for terminating Child:\n";
        int retParent = 2;
        TerminateProcess(pi.hProcess, retParent);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "\n\n### After Killer" << std::endl;

    showAfterKill(vecForName, vecForId);
}