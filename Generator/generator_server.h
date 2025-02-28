#pragma once

#include "generator.h"

#include <ipc.h>
#include <shared_data.h>

class Server : public IPC::ServerBase<Server, SharedData>
    , public RandomPairGenerator {

    using ServerBase = IPC::ServerBase<Server, SharedData>;
    using RandGenerator = RandomPairGenerator;

public:
    Server(const std::chrono::nanoseconds& i_ns
        , const std::optional<std::wstring>& i_csv_path);

    bool Init();
    void Start();
};