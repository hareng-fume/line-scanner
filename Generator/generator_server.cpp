#include "generator_server.h"

#include <iostream>

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
ServerPairGenerator::ServerPairGenerator(const std::chrono::nanoseconds& i_ns
    , const std::optional<std::wstring>& i_csv_path)
    : ServerBase(i_ns)
    , RandGenerator(i_csv_path) {}

//-----------------------------------------------------------------------------
bool ServerPairGenerator::Init() {
    if (!ServerBase::Init())
        return false;

    auto p_data = _GetData();
    p_data->rowIndex.store(0, std::memory_order_relaxed);
    p_data->processIndex.store(0, std::memory_order_release);
    return true;
}

//-----------------------------------------------------------------------------
void ServerPairGenerator::Start() {
    auto generator = Generate();

    auto p_data = _GetData();
    while (generator.next()) {
        auto r_idx = p_data->rowIndex.load(std::memory_order_relaxed);

        auto [x, y] = generator.value();
        p_data->buffer[r_idx][0] = x;
        p_data->buffer[r_idx][1] = y;

        // signal that the data is ready for processing
        p_data->processIndex.store(1, std::memory_order_release);

        std::cout << "Server: Generated (" << x << ", " << y << ")\n";

        // wait until the Processor consumes the data and signals readiness for new data
        while (p_data->processIndex.load(std::memory_order_acquire) != 0)
            _mm_pause();  // reduce CPU load during waiting

        // update row index for the next set of data
        p_data->rowIndex.store((r_idx + 1)%data_type::rows_value, std::memory_order_release);
    }
}

//-----------------------------------------------------------------------------
SharedPair* ServerPairGenerator::_GetData() const {
    return reinterpret_cast<SharedPair*>(mp_data);
}

// TODO: maybe remove
/*
//-----------------------------------------------------------------------------
ServerRandBufferGenerator::ServerRandBufferGenerator(const std::chrono::nanoseconds& i_ns)
    : ServerBase(i_ns), UniformDistr(0, 255) {
}

// TODO: maybe remove
//-----------------------------------------------------------------------------
void ServerRandBufferGenerator::Start() {
    while (true) {
        int rowIdx = _GetData()->rowIndex.load(std::memory_order_relaxed);

        // Write a new row
        for (int col = 0; col < _COLS; col++)
            _GetData()->buffer[rowIdx % _ROWS][col] = Draw();
        
        _GetData()->rowIndex.store(rowIdx + 1, std::memory_order_release);

        std::cout << "Generated Row " << rowIdx << ": ";
        for (int col = 0; col < _COLS; col++)
            std::cout << _GetData()->buffer[rowIdx % _ROWS][col] << " ";
        std::cout << "\n";

        Sleep(50);  // Simulate processing delay
    }
}

// TODO: maybe remove
//-----------------------------------------------------------------------------
SharedDataBuffer* ServerRandBufferGenerator::_GetData() const {
    return reinterpret_cast<SharedDataBuffer*>(mp_data);
}
*/