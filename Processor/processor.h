#pragma once

#include <generator_coro.h>

#include <utility>

struct SharedDataPair;

// TODO: either add filter logic or remove
class RandomPairProcessor {

    using TGenerator = Generator<std::pair<int, int>>;

public:
    TGenerator Process(SharedDataPair* ip_data);
};