#pragma once

#include "processor.h"

#include <ipc.h>
#include <shared_data.h>

//-----------------------------------------------------------------------------
class ClientPairProcessor : public IPC::ClientBase<SharedPair>
    , public RandomPairProcessor {

    using ClientBase = IPC::ClientBase<SharedPair>;
    using RandProcessor = RandomPairProcessor;

public:
    ClientPairProcessor(const std::chrono::nanoseconds& i_ns
        , const double& i_threshold);

    void Start() override;

private:
    SharedPair* _GetData() const;

private:
    double m_threshold;
};

/*
//-----------------------------------------------------------------------------
class ClientBufferProcessor : public IPC::ClientBase<SharedDataBuffer> {

    using ClientBase = IPC::ClientBase<SharedDataBuffer>;

public:
    ClientBufferProcessor(const std::chrono::nanoseconds& i_ns
        , const double& i_threshold);

    void Start() override;

private:
    SharedDataBuffer* _GetData() const;

private:
    double m_threshold;
};
*/