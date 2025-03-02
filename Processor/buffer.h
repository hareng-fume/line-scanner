#pragma once

#include <utils.h>

#include <array>
#include <cassert>

#include <immintrin.h>  // AVX2

template <size_t SIZE>
class Buffer {

public:
    template <typename T>
    void Add(T i_val) {
        static_assert(std::is_constructible_v<double, T>, "double is not constructible from T");

        if (!isReady) {
            data[++startIdx] = i_val;

            if (startIdx == SIZE-1) {
                isReady = true;
                startIdx = 0;
            }
        }
        else {
            startIdx = startIdx > 0 ? startIdx - 1 : SIZE - 1;
            data[startIdx] = i_val;
        }
    }

    template <size_t F_SIZE>
    double FilterAVX2(const double(&i_filter)[F_SIZE]) const {
        static_assert(SIZE == F_SIZE, "Filter size mismatch");

        __m256d vec1, vec2, result = _mm256_setzero_pd();

        int i = 0;
        for (; i + 4 <= SIZE; i += 4) {          // process in chunks of 4
            vec1 = _mm256_loadu_pd(&data[i]);
            vec2 = _mm256_loadu_pd(&i_filter[i]);

            // multiply element-wise and accumulate
            result = _mm256_add_pd(result, _mm256_mul_pd(vec1, vec2));
        }

        // extract partial sum
        double sum = ((double*)&result)[0] + ((double*)&result)[1] +
            ((double*)&result)[2] + ((double*)&result)[3];

        // handle remainder elements (SIZE % 4)
        for (; i < SIZE; i++)
            sum += static_cast<double>(data[i]) * i_filter[i];

        return sum;
    }

    bool IsReady() const {
        return isReady;
    }

private:
    alignas(32) double data[SIZE] = {};
    int startIdx = -1;
    bool isReady = false;
};