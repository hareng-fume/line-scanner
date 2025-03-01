#pragma once

#include <generator_coro.h>

#include <utility>

struct SharedData;

class RandomPairProcessor {

    using TGenerator = Generator<std::pair<int, int>>;

public:
    TGenerator Process(SharedData* ip_data);
};