#pragma once

#include <atomic>

template <int ROWS, int COLS>
struct alignas(64) SharedData {

    static constexpr const int rows_value = ROWS;
    static constexpr const int cols_value = COLS;

    std::atomic<unsigned long> rowIndex; // index for generating new random row
    std::atomic<int> processIndex;       // flag to synchronize processes (0 = ready, 1 = processed)
    int buffer[ROWS][COLS];

    static constexpr const wchar_t* NAME = L"Local\\RandStreamSharedMemory";
};

using SharedPair = SharedData<16, 2>;
