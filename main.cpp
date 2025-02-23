#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <random>
#include <string>
#include <string_view>
#include <unordered_map>

#include <iostream>
#include <cassert>
#include <cstdlib>

namespace {

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
    #ifdef _DEBUG
        _PrintArgs(argc, argv);
    #endif
    */

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


/*
#ifdef _DEBUG
    _PrintArgs(args);
#endif
*/

    return EXIT_SUCCESS;
}
