#pragma once

#include "generator.h"

#include <ipc.h>
#include <shared_data.h>

//-----------------------------------------------------------------------------
class ServerPairGenerator : public IPC::ServerBase<SharedPair>
    , public RandomPairGenerator {

    using ServerBase = IPC::ServerBase<SharedPair>;
    using RandGenerator = RandomPairGenerator;

public:
    ServerPairGenerator(const std::chrono::nanoseconds& i_ns
        , const std::optional<std::wstring>& i_csv_path);

    bool Init() override;
    void Start() override;

private:
    SharedPair* _GetData() const;
};

// TODO: maybe remove
/*
//-----------------------------------------------------------------------------
class ServerRandBufferGenerator : public IPC::ServerBase<SharedDataBuffer>
    , public UniformDistr {

    using ServerBase = IPC::ServerBase<SharedDataBuffer>;

public:
    explicit ServerRandBufferGenerator(const std::chrono::nanoseconds& i_ns);

    void Start() override;

private:
    SharedDataBuffer* _GetData() const;
};
*/