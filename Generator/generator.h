#pragma once

#include <generator_coro.h>

#include <string>
#include <fstream>
#include <random>
#include <optional>

//-----------------------------------------------------------------------------
class RandomPairGenerator {

    using TGenerator = Generator<std::pair<int, int>>;

public:
    explicit RandomPairGenerator(const std::optional<std::wstring>& i_file_name = std::nullopt);

    TGenerator Generate();

private:
    bool file_mode;
    std::ifstream file;
    std::random_device rd; // a seed source for the random number engine
    std::mt19937 gen;      // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib;
};