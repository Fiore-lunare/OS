#include <iostream>
#include <windows.h>

using namespace std;

int jump(STARTUPINFOA& startInfoProcess, PROCESS_INFORMATION& process_Info, HANDLE pipe1, HANDLE pipe2, std::string str) {
    GetStartupInfoA(&startInfoProcess);
    startInfoProcess.hStdInput = pipe1;
    startInfoProcess.hStdOutput = pipe2;
    startInfoProcess.dwFlags = STARTF_USESTDHANDLES;
    startInfoProcess.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    BOOL prM = CreateProcessA(NULL, (LPSTR)str.c_str(),
        NULL, NULL,
        TRUE,
        0,
        NULL,
        NULL,
        &startInfoProcess,
        &process_Info);
    if (!prM)
    {
        cout << "Could not create processM" << std::endl;
        return 1;
    }
    return 0;
}


int main()
{
    SECURITY_ATTRIBUTES saFile = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE hFileIn = CreateFile(L"Input.txt", GENERIC_READ, 0, &saFile,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFileIn == INVALID_HANDLE_VALUE)
    {
        cout << "Could not open file Output.txt" << std::endl;
        return 1;
    }

    DWORD nsize = 100;
    SECURITY_ATTRIBUTES saPipe =
    { sizeof(SECURITY_ATTRIBUTES),NULL,TRUE };
    
   
    HANDLE hWritePipeMA = NULL, hReadPipeMA = NULL;

    BOOL pipeMA = CreatePipe(&hReadPipeMA, &hWritePipeMA, &saPipe, nsize);
    if (!pipeMA)
    {
        cout<<"Could not create PipeMA"<<std::endl;
        return 1;
    }

    PROCESS_INFORMATION processM_Info, processA_Info, processP_Info, processS_Info;
    STARTUPINFOA startInfoProcessM, startInfoProcessA, startInfoProcessP, startInfoProcessS;

    if (jump(startInfoProcessM, processM_Info, hFileIn, hWritePipeMA, "ProjectM.exe") != 0) return 1;
    CloseHandle(hWritePipeMA);


    HANDLE hWritePipeAP = NULL, hReadPipeAP = NULL;

    BOOL pipeAP = CreatePipe(&hReadPipeAP, &hWritePipeAP, &saPipe, nsize);
    if (!pipeAP)
    {
        cout << "Could not create PipeAP" << std::endl;
        return 1;
    }


    if (jump(startInfoProcessA, processA_Info, hReadPipeMA, hWritePipeAP, "ProjectA.exe") != 0)  return 1;
    CloseHandle(hWritePipeAP);
    CloseHandle(hReadPipeMA);


    HANDLE hWritePipePS =NULL, hReadPipePS =NULL;

    BOOL pipePS = CreatePipe(&hReadPipePS, &hWritePipePS, &saPipe, nsize);
    if (!pipePS)
    {
        cout << "Could not create PipeAP" << std::endl;
        return 1;
    }

    if (jump(startInfoProcessP, processP_Info, hReadPipeAP, hWritePipePS, "ProjectP.exe") != 0)  return 1;
    CloseHandle(hWritePipePS);
    CloseHandle(hReadPipeAP);

    if (jump(startInfoProcessS, processS_Info, hReadPipePS, GetStdHandle(STD_OUTPUT_HANDLE), "ProjectS.exe") != 0)  return 1;
    CloseHandle(hReadPipePS);
    CloseHandle(hFileIn);

    HANDLE pr[] = { processM_Info.hProcess, processA_Info.hProcess, processP_Info.hProcess, processS_Info.hProcess };
    WaitForMultipleObjects(4, pr, TRUE, INFINITE);
    
    CloseHandle(processA_Info.hProcess);  
    CloseHandle(processA_Info.hThread);

    CloseHandle(processP_Info.hProcess);
    CloseHandle(processP_Info.hThread);

    CloseHandle(processS_Info.hProcess);
    CloseHandle(processS_Info.hThread);

    CloseHandle(processM_Info.hProcess);
    CloseHandle(processM_Info.hThread);
}

