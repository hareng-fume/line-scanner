#pragma once

#include <atomic>

struct alignas(64) SharedData {
    int x, y;              // (x, y) pair
    std::atomic<int> flag; // 0 = writing, 1 = ready to read

    static constexpr const wchar_t* NAME = L"Local\\RandStreamSharedMemory";
};


#define ROWS 16  // Circular buffer size (power of 2 for indexing)
#define COLS 9   // Example: 5 columns per row

struct _SharedData {
    std::atomic<int> rowIndex{ 0 }; // Atomic index for writing/reading rows
    int buffer[ROWS][COLS];         // Circular row buffer

    static constexpr const wchar_t* g_name = L"Global\\RandStreamSharedMemory";
};

