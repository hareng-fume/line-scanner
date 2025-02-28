#include <sstream>
#include <iostream>

#include <windows.h>

namespace {

    //-------------------------------------------------------------------------
    static void _StartProcess(const std::wstring& executable
        , const std::wstring& arguments
        , HANDLE& processHandle) {

        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        // Set up the STARTUPINFO structure
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;

        std::wstring commandLine = executable + L" " + arguments;
        std::wcout << L"CreateProcess Args: " << commandLine << std::endl;
        if (!CreateProcess(
            NULL,                          // No module name (use command line)
            &commandLine[0],               // Command line
            NULL,                          // Process handle not inheritable
            NULL,                          // Thread handle not inheritable
            FALSE,                         // Set handle inheritance to FALSE
            CREATE_NEW_CONSOLE,
            //DETACHED_PROCESS,              // No creation flags
            NULL,                          // Use parent's environment block
            NULL,                          // Use parent's starting directory
            &si,                           // Pointer to STARTUPINFO structure
            &pi                            // Pointer to PROCESS_INFORMATION structure
        )) {
            std::wcerr << L"Failed to start process: " << executable
                << L" with error: " << GetLastError() << std::endl;
        }
        else {
            std::wcout << "Process started: " << executable << std::endl;
            processHandle = pi.hProcess;
            CloseHandle(pi.hThread);
        }
    }

} // namespace

int wmain(int argc, wchar_t* argv[]) {

    constexpr const wchar_t* generatorPath = L"Generator.exe";
    constexpr const wchar_t* processorPath = L"Processor.exe";

    // build command line string
    std::wstringstream cmd;
    for (int i = 1; i < argc; ++i) {
        if (i > 1)
            cmd << L" "; // space between arguments

        // if the argument contains a space, wrap it in quotes
        if (wcschr(argv[i], L' '))
            cmd << L"\"" << argv[i] << L"\"";
        else
            cmd << argv[i];
    }

    std::wcout << L"Launching Generator and Processor...\n";

    HANDLE processes[] = {nullptr, nullptr};
    _StartProcess(processorPath, cmd.str(), processes[0]);
    _StartProcess(generatorPath, cmd.str(), processes[1]);


    // optionally wait for both processes to finish
    WaitForMultipleObjects(2, processes, TRUE, INFINITE);

    // close process handles after they exit
    CloseHandle(processes[0]);
    CloseHandle(processes[1]);

    std::wcout << L"Processes finished.  \n";
    return EXIT_SUCCESS;
}
