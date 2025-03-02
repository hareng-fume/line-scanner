#pragma once

#include <atomic>

template <int ROWS, int COLS>
struct alignas(32) SharedData {

    using value_type = int;

    static constexpr const int rows_value = ROWS;
    static constexpr const int cols_value = COLS;

    std::atomic<unsigned long> rowIndex; // index for generating new random row
    std::atomic<int> processIndex;       // flag to synchronize processes (0 = ready, 1 = processed)
    value_type buffer[ROWS][COLS];

    static constexpr const wchar_t* NAME = L"Local\\RandStreamSharedMemory";
};

using SharedPair = SharedData<16, 2>;
