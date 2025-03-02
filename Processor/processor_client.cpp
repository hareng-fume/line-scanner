#include "processor_client.h"
#include "buffer.h"

#include <immintrin.h>

namespace {

    alignas(32) static constexpr const double _FILTER[] = {
        0.00025177, 0.008666992, 0.078025818, 0.24130249, 0.343757629, 0.24130249, 0.078025818, 0.008666992, 0.000125885
    };

} // namespace

//-----------------------------------------------------------------------------
ClientPairProcessor::ClientPairProcessor(const std::chrono::nanoseconds& i_ns
    , const double& i_threshold)
    : ClientBase(i_ns)
    , m_threshold(i_threshold) {}

//-----------------------------------------------------------------------------
void ClientPairProcessor::Start() {

    Buffer<utils::get_size(_FILTER)> buffer;
    auto p_data = _GetData();

    while (true) {

        // wait until the data is ready from the Generator
        while (p_data->processIndex.load(std::memory_order_acquire) != 1)
            _mm_pause();  // Reduce CPU load during waiting

        // Process the entire buffer of random pairs (x, y)
        int rowIdx = p_data->rowIndex.load(std::memory_order_relaxed);

        for (int colIdx = 0; colIdx < p_data->cols_value; ++colIdx) {
            int val = p_data->buffer[rowIdx][colIdx];
            buffer.Add(val);
            if (buffer.IsReady()) {
                double filtered_val = buffer.FilterAVX2(_FILTER);
                filtered_val < m_threshold ? 0 : 1;
            }
        }

        // signal that the Processor has consumed the data and the Generator can proceed
        p_data->processIndex.store(0, std::memory_order_release);
    }
}

//-----------------------------------------------------------------------------
SharedPair* ClientPairProcessor::_GetData() const {
    return reinterpret_cast<SharedPair*>(mp_data);
}

/*
//-----------------------------------------------------------------------------
ClientBufferProcessor::ClientBufferProcessor(const std::chrono::nanoseconds& i_ns
    , const double& i_threshold)
    : ClientBase(i_ns)
    , m_threshold(i_threshold) {}

//-----------------------------------------------------------------------------
void ClientBufferProcessor::Start() {

    while (true) {
        int rowIdx = _GetData()->rowIndex.load(std::memory_order_acquire);

        // ensure at least 3 rows have been written before processing
        if (rowIdx < 3) {
            _mm_pause();
            continue;
        }

        // Apply a simple filter on the last 3 rows (Moving Average per column)
        float avg[_COLS] = { 0 };
        for (int i = 0; i < 3; i++) {
            int r = (rowIdx - 3 + i) % _ROWS;
            for (int col = 0; col < _COLS; col++) {
                avg[col] += _GetData()->buffer[r][col];
            }
        }
        for (int col = 0; col < _COLS; col++) {
            avg[col] /= 3.0f;
        }

        std::cout << "Filtered Values (Avg of last 3 rows): ";
        for (int col = 0; col < _COLS; col++) {
            std::cout << avg[col] << " ";
        }
        std::cout << "\n";

        Sleep(100);  // Simulate processing delay
    }

}

SharedDataBuffer* ClientBufferProcessor::_GetData() const {
    return reinterpret_cast<SharedDataBuffer*>(mp_data);
}
*/