#include "processor.h"

#include <shared_data.h>

// TODO: either add filter logic or remove
RandomPairProcessor::TGenerator RandomPairProcessor::Process(SharedDataPair* ip_data) {
    /*while (true) {
        while (ip_data->flag.load(std::memory_order_acquire) == 0);

        int x = ip_data->x;
        int y = ip_data->y;
        ip_data->flag.store(0, std::memory_order_release); // Mark as read

        co_yield{ x, y }; // Coroutine returns the value
    }*/
    co_yield {};
}