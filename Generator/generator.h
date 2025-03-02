#pragma once

#include <generator_coro.h>

#include <string>
#include <fstream>
#include <random>
#include <optional>

//-----------------------------------------------------------------------------
struct UniformDistr {
public:
    UniformDistr(int i_min, int i_max);
    int Draw();

private:
    std::mt19937 m_gen;
    std::uniform_int_distribution<int> m_distrib;
};

//-----------------------------------------------------------------------------
class RandomPairGenerator : public UniformDistr {

    using TGenerator = Generator<std::pair<int, int>>;

public:
    explicit RandomPairGenerator(const std::optional<std::wstring>& i_file_name = std::nullopt);

    TGenerator Generate();

private:
    bool file_mode;
    std::ifstream file;
};