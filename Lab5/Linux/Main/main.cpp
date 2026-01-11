#include <iostream>
using namespace std;

#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <sys/wait.h>


int jump (int pipe1, int pipe2, const std::string &prog){
    pid_t pid = fork();
    if (pid == -1) {
        perror("Project_app failed");
        return 1;
    }
    if (pid == 0) {
        if (dup2(pipe1, STDIN_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        if (dup2(pipe2, STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipe1);
        if (pipe2 != STDOUT_FILENO) close(pipe2);
        std::vector<char*> args;
        args.push_back(const_cast<char*>(prog.c_str()));
        args.push_back(nullptr);
        execvp(prog.c_str(), args.data());
        perror("Execvp for Program failed");
        exit(1);
    }
    close(pipe1);
    if (pipe2 != STDOUT_FILENO) close(pipe2);
    return 0;
}

int main() {
    int fileIn = open("Input.txt", O_RDONLY);
    if (fileIn == -1) {
        std::cout<<"Failed to open file"<<std::endl;
        return 1;
    }

    int pipeMA[2];
    if (pipe(pipeMA) == -1){
        std::cout << "Could not create PipeMA" << std::endl;
        return 1;
    }

    if (jump(fileIn, pipeMA[1], "./ProjectM_app") != 0) return 1;
    int pipeAP[2];

    if (pipe(pipeAP) == -1){
        std::cout << "Could not create PipeAP" << std::endl;
        return 1;
    }
    if (jump(pipeMA[0], pipeAP[1], "./ProjectA_app") != 0) return 1;

    int pipePS[2];
    if (pipe(pipePS) == -1){
        std::cout << "Could not create PipePS" << std::endl;
        return 1;
    }
    if (jump(pipeAP[0], pipePS[1], "./ProjectP_app") != 0) return 1;
    if (jump(pipePS[0],STDOUT_FILENO,  "./ProjectS_app") != 0 ) return 1;

    for (int i = 0; i < 4; ++i) wait(nullptr);

    return 0;
}