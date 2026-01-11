#include <iostream>

#include <sstream>
#include <regex>
#include <thread>
#include <chrono>
#include <libproc.h>
#include <signal.h>
#include <string>
#include <unistd.h>
#include <vector>

bool checkName(std::string& str) {
    std::regex pattern("([^\\>\\<\\:\\\"\\/\\\\\\|\\?\\*]+\\|)*([^\\>\\<\\:\\\"\\/\\\\\\|\\?\\*])+");
    return std::regex_match(str, pattern) || str == "";
}

bool checkId(std::string& str) {
    std::regex pattern("((\\d+\\|)*\\d+)");
    return std::regex_match(str, pattern) || str == "";
}

bool byId(pid_t processId) {
    if (kill(processId, 0) ==0 ) return 1;
    return 0;
}

bool checkPrExId(std::string& str, std::vector<pid_t>& vec) {
    int processId = stoi(str);
    if (!byId(processId)) return 0;
    vec.push_back(processId);
    return 1;
}


bool checkPrExName(std::string& str, std::vector<std::pair<pid_t, std::string> >& vec) {
    int count = proc_listpids(PROC_ALL_PIDS, 0, NULL,0 );
    if (count < 0 ) return 0;
    std::vector<pid_t> pids(count + 15);
    count = proc_listpids(PROC_ALL_PIDS, 0, pids.data(), pids.size()* sizeof(pid_t));
    char nameBuffer[PROC_PIDPATHINFO_MAXSIZE];
    for (int i =0; i< count; i++) {
        if (pids[0] ==0) continue;
        if (proc_name(pids[i], nameBuffer, sizeof(nameBuffer)) > 0) {
            if (str== nameBuffer) {
                vec.push_back(std::make_pair(pids[i], str));
                return 1;
            }
        }
    }
    return  0;
}


void parseAndCheckId(std::string& str, std::vector<pid_t>& vec) {
    std::stringstream ss(str);
    std::string curWord;

    while (std::getline(ss, curWord, '|')) {
        if (checkPrExId(curWord, vec)) {
            std::cout << "process with ID = " << curWord << " exists"<<std::endl;
        }
        else std::cout << "process with ID = " << curWord << " doesn't exist"<<std::endl;
    }
}

int createCommandId(std::string& str) {
    std::cout << "\n-------Enter process id to kill-------"<<std::endl;
    std::getline(std::cin, str);
    if (!checkId(str)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    return 0;
}

void parseAndCheckName(std::string& str, std::vector<std::pair<pid_t, std::string> >& vec) {
    std::stringstream ss(str);
    std::string curWord;
    while (std::getline(ss, curWord, '|')) {
        if (checkPrExName(curWord, vec)) {
            std::cout << "process with name = " << curWord << " exists"<<std::endl;
        }
        else std::cout << "process with name = " << curWord << " doesn't exist"<<std::endl;
    }
}

int createCommandName(std::string& str) {
    std::cout << "\n-------Enter process name to kill-------"<<std::endl;
    std::getline(std::cin, str);
    if (!checkName(str)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    return 0;
}

bool setEnvironmentVar(std::string& envVar) {
    std::cout << "Enter PROC_TO_KILL\nx1|x2|...|xn"<<std::endl;
    std::getline(std::cin, envVar);
    if (!checkName(envVar)) {
        std::cout << "Incorrect input" << std::endl;
        return 1;
    }
    int res = setenv("PROC_TO_KILL", envVar.c_str(), 1);
    if (res)
    {
        std::cout << "Unable to Set Environment Variable" << std::endl;
        return 1;
    }
    return 0;

}

void showAfterKill(std::vector < std::pair < pid_t, std::string> >& vecForName, std::vector<pid_t>& vecForId) {
    for (auto elem : vecForName) {
        bool res = byId(elem.first);
        if (res) std::cout << "process with Name = " << elem.second << " exists"<<std::endl;
        else std::cout << "process with name = " << elem.second << " doesn't exist"<<std::endl;
    }

    for (auto elem : vecForId) {
        bool res = byId(elem);
        if (res) std::cout << "process with ID = " << elem << " exists"<<std::endl;
        else std::cout << "process with ID = " << elem << " doesn't exist"<<std::endl;
    }
}


int main() {
    std::string name = "", id = "", envVar = "", prog = "./killer_app";
    std::vector<char*> args;
    args.push_back(const_cast<char*>(prog.c_str()));

    std::vector <std::pair <pid_t, std::string>> vecForName ; std::vector<pid_t> vecForId;

    if (setEnvironmentVar(envVar)) return 1;
    parseAndCheckName(envVar, vecForName);
    if (createCommandId(id)) return 1;
    parseAndCheckId(id, vecForId);
    if (createCommandName(name)) return 1;
    parseAndCheckName(name, vecForName);
    std::vector<std::string> stringStorage;

    for (pid_t id : vecForId) {
        stringStorage.push_back(std::to_string(id));
    }

    char* NAME = "--name";
    char* ID = "--id";

    for (int i =0; i<vecForName.size(); i++) {
        args.push_back(NAME);
        args.push_back(const_cast<char*>(vecForName[i].second.c_str()));
    }

    for (int i =0; i<vecForId.size(); i++) {
        args.push_back(ID);
        args.push_back(const_cast<char*>(stringStorage[i].c_str()));
    }

    args.push_back(NULL);
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }
    if (pid == 0) {
        execvp(prog.c_str(), args.data());
        perror("Execvp failed");
        exit(1);
    }
    waitpid(pid, nullptr,0);
    showAfterKill(vecForName, vecForId);
}