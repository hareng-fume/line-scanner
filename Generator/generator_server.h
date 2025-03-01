#pragma once

#include "generator.h"

#include <ipc.h>
#include <shared_data.h>

class Server : public IPC::ServerBase<SharedData>
    , public RandomPairGenerator {

    using ServerBase = IPC::ServerBase<SharedData>;
    using RandGenerator = RandomPairGenerator;

public:
    Server(const std::chrono::nanoseconds& i_ns
        , const std::optional<std::wstring>& i_csv_path);

    bool Init() override;
    void Start() override;

private:
    SharedData* _GetData() const;
};