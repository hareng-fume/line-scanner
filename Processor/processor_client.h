#pragma once

#include "processor.h"

#include <ipc.h>
#include <shared_data.h>


class Client : public IPC::ClientBase<SharedData>
    , public RandomPairProcessor {

    using ClientBase = IPC::ClientBase<SharedData>;
    using data_type = ClientBase::data_type;

    using RandProcessor = RandomPairProcessor;

public:
    Client(const std::chrono::nanoseconds& i_ns
        , const double& i_threshold);

    void Start() override;

private:
    double m_threshold;
};