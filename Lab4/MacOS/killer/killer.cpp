#include <iostream>
#include <signal.h>
#include <sstream>
#include <libproc.h>
#include <unistd.h>

bool killById(int id) {
    int res = kill(id, SIGKILL);
    return res;
}

void killByName(const std::string& nameToFind) {
    int count = proc_listpids(PROC_ALL_PIDS, 0, NULL,0 );
    if (count < 0 ) return;
    std::vector<pid_t> pids(count + 15);
    count = proc_listpids(PROC_ALL_PIDS, 0, pids.data(), pids.size()* sizeof(pid_t));
    char nameBuffer[PROC_PIDPATHINFO_MAXSIZE];
    for (int i =0; i< count; i++) {
        if (pids[0] ==0) continue;
        if (proc_name(pids[i], nameBuffer, sizeof(nameBuffer)) > 0) {
            if (nameToFind == nameBuffer) {
                killById(pids[i]);
            }
        }
    }
}

void killFromEnv(){
    const char* env = std::getenv("PROC_TO_KILL");
    if (!env) return;
    std::stringstream ss(env);
    std::string temp;

    while (getline(ss, temp, ',')) {
        killByName(temp);
    }
}

int main(int argc, char* argv[]){
    for (int i = 1; i < argc; i++)
    {
        std::string argument = argv[i];
        if (argument == "--id") {
            if (i + 1 < argc) {
                i++;
                std::string argument = argv[i];
                killById(stoi(argument));
            }
        }
        else if (argument == "--name") {
            if (i + 1 < argc) {
                i++;
                std::string argument = argv[i];
                killByName(argument);
            }
        }

        else {
            std::cout<<"Warning: Unknown argument '" << argv[i]<< "\' ignored."<<std::endl;
        }
    }
    killFromEnv();

}