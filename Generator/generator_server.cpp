#include "generator_server.h"

#include <iostream>

using namespace std::chrono_literals;

//-----------------------------------------------------------------------------
Server::Server(const std::chrono::nanoseconds& i_ns
    , const std::optional<std::wstring>& i_csv_path)
    : ServerBase(i_ns)
    , RandGenerator(i_csv_path) {}

//-----------------------------------------------------------------------------
bool Server::Init() {
    if (!ServerBase::Init())
        return false;

    _GetData()->flag.store(0, std::memory_order_release);
    return true;
}

//-----------------------------------------------------------------------------
void Server::Start() {
    auto generator = Generate();

    while (generator.next()) {
        // Wait until previous data is read
        while (_GetData()->flag.load(std::memory_order_acquire) == 1);

        auto [x, y] = generator.value();
        _GetData()->x = x;
        _GetData()->y = y;
        _GetData()->flag.store(1, std::memory_order_release); // Mark data as ready

        std::cout << "Server: Generated (" << x << ", " << y << ")\n";
        Sleep(1); // Simulate small delay
    }
}

//-----------------------------------------------------------------------------
SharedData* Server::_GetData() const {
    return reinterpret_cast<SharedData*>(mp_data);
}