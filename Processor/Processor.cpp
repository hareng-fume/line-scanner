// Processor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <shared_data.h>
#include <utils.h>

#include <array>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
    static int _get_threshold(std::unordered_map<std::wstring, std::wstring>&& args) {
        if (args.count(utils::THRESHOLD) > 0)
            return std::stoi(args[utils::THRESHOLD]);
        throw std::runtime_error("No threshold value provided.");
    }

    inline static void _print_context(int threshold) {
        std::cout << "Threshold: " << threshold << std::endl;
    }

    static constexpr const std::array<double, 9> kernel = {
        0.00025177, 0.008666992, 0.078025818, 0.24130249, 0.343757629, 0.24130249, 0.078025818, 0.008666992, 0.000125885
    };

    // threshold hint:
    //      min averaged filtered value: 124.027804061529
    //      max averaged filtered value: 131.1727234194942

} // namespace

int wmain(int argc, wchar_t* argv[]) {
    std::wcout << "Processor started" << std::endl;

    auto args = utils::_parse_args(argc, argv);
    auto threshold = _get_threshold(std::move(args));
    _print_context(threshold);

    /*utils::unique_handle map_file;
    if (HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, SharedData::g_name); hMapFile) {
        map_file.reset(hMapFile);
    }
    else {
        std::wcerr << L"Error opening shared memory\n";
        return EXIT_FAILURE;
    }

    utils::unique_sd data;
    if (auto pData = MapViewOfFile(map_file.get(), FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedData)); pData) {
        data.reset(static_cast<SharedData*>(pData));
    }
    else {
        std::wcerr << L"Error mapping shared memory\n";
        return EXIT_FAILURE;
    }

    while (true) {
        int rowIdx = data->rowIndex.load(std::memory_order_acquire);

        data->buffer[rowIdx];
    }


    std::this_thread::sleep_for(10s);*/
    return EXIT_SUCCESS;
}
