#include "generator_server.h"

#include <iostream>

//-----------------------------------------------------------------------------
Server::Server(const std::chrono::nanoseconds& i_ns
    , const std::optional<std::wstring>& i_csv_path)
    : ServerBase(i_ns)
    , RandGenerator(i_csv_path) {}

//-----------------------------------------------------------------------------
bool Server::Init() {
    pData->flag.store(0, std::memory_order_release);
    return true;
}

//-----------------------------------------------------------------------------
void Server::Start() {
    auto generator = Generate();

    while (generator.next()) {
        // Wait until previous data is read
        while (pData->flag.load(std::memory_order_acquire) == 1);

        auto [x, y] = generator.value();
        pData->x = x;
        pData->y = y;
        pData->flag.store(1, std::memory_order_release); // Mark data as ready

        std::cout << "Server: Generated (" << x << ", " << y << ")\n";
        Sleep(50); // Simulate small delay
    }
}