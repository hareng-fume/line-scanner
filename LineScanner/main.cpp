#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>

#include <iostream>
#include <cassert>
#include <cstdlib>

#include <windows.h>

namespace {

    //-------------------------------------------------------------------------
    static void _StartProcess(const std::wstring& executable, const std::wstring& arguments) {
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };

        // Set up the STARTUPINFO structure
        si.cb = sizeof(STARTUPINFO);
        si.dwFlags = STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_SHOW;

        // Create the process (Generator or Processor)
        std::wstring commandLine = executable + L" " + arguments;
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
            std::wcerr << L"CreateProcess failed for " << executable
                << L" with error: " << GetLastError() << std::endl;
            return;
        }

        // Wait for the process to complete
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    //-------------------------------------------------------------------------
    static std::wstring _ToWString(const std::string& a_string) {
        // Get the size required for the wide-character string (including null-terminator)
        auto size = MultiByteToWideChar(CP_UTF8, 0, a_string.c_str(), -1, nullptr, 0);
        std::wstring w_string(size, 0);

        // Perform the conversion
        MultiByteToWideChar(CP_UTF8, 0, a_string.c_str(), -1, &w_string[0], size);
        return w_string;
    }


    //-------------------------------------------------------------------------
#ifdef _DEBUG
    static void _PrintArgs(const std::unordered_map<std::string, std::string>& args) {
        for (const auto& [key, val] : args)
            std::cout << key << " " << val << "\n";
    }
    static void _PrintArgs(int argc, char* argv[]) {
        for (int i = 1; i < argc; ++i)
            std::cout << argv[i] << "\n";
    }
#endif

    static constexpr unsigned long _INF = 10;

    inline static void _Error(const std::string& msg) {
        std::cerr <<  msg << std::endl;
    }

    static bool _GenerateTestCSV(int columns) {
        std::ofstream outfile("test.csv");
        if (!outfile.is_open()) {
            _Error("Error opening file: test.csv");
            return false;
        }

        std::random_device rd;  // a seed source for the random number engine
        std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<> distrib(0, 255);
        auto generator = [&distrib, &gen](){ return distrib(gen); };

        std::vector<int> nums(columns);
        for(int r = 0; r < _INF; ++r) {
            std::generate(nums.begin(), nums.end(), generator);
            for (int c = 0; c < columns; ++c) {
                if (c < columns-1)
                    outfile << nums[c] << ',';
                else
                    outfile << nums[c] << '\n';
            }
        }

        return true;
    }


    // e.g. a.exe --test --columns=100 --threshold=1 --time-limit=500
    static std::unordered_map<std::string, std::string> _Parse(int argc, char* argv[]) {
        std::unordered_map<std::string, std::string> args;
        for (int i = 1; i < argc; ++i) {
            std::string_view arg = argv[i];
            bool ok = false;
            if (auto pos = arg.find('='); pos != arg.npos) {
                auto first = arg.find_first_not_of('-');
                if (first != arg.npos) {
                    std::string key(arg.substr(first, pos-first));
                    std::string val(arg.substr(pos+1));
                    if (args.count(key) == 0) {
                        args.insert({std::move(key), std::move(val)});
                        ok = true;
                    }
                }
            }
            else if (arg == "--test") {
                args["test"] = "true";
                ok = true;
            }

            if (!ok)
                _Error("Can't handle argument: "+ std::string(arg));
        }
        return args;
    }
}

int main(int argc, char* argv[]) {

    /*
    auto args = _Parse(argc, argv);
    if (args.size() != argc - 1)
        return EXIT_FAILURE;


    if (args.count("test") > 0) {
        if (args.count("columns") == 0) {
            _Error("Test mode implies auto test data generation, please check --columns=<val> arg setup");
            return EXIT_FAILURE;
        }
        _GenerateTestCSV(std::atoi(args["columns"].c_str()));
    }
    */

    // Define the paths to your executables
    constexpr const wchar_t* generatorPath = L"Generator.exe";
    constexpr const wchar_t* processorPath = L"Processor.exe";


    std::stringstream ss;
    for (int i = 1; i < argc; ++i) {
        ss << argv[i];
        if (i < argc-1)
           ss << " ";
    }
    auto wargs = _ToWString(ss.str());


    std::cout << "Launching Generator and Processor...\n";

    _StartProcess(generatorPath, wargs);
    _StartProcess(processorPath, wargs);

    std::cout << "Processes started!\n";

    return EXIT_SUCCESS;
}
