#include "processor_client.h"

//-----------------------------------------------------------------------------
Client::Client(const std::chrono::nanoseconds& i_ns
    , const double& i_threshold)
    : ClientBase(i_ns)
    , m_threshold(i_threshold) {}

//-----------------------------------------------------------------------------
void Client::Start() {
    auto processor = Process(_GetData());

    while (processor.next()) {
        auto [x, y] = processor.value();
        std::cout << "Client: Asynchronously processed (" << x << ", " << y << ")\n";
    }
}

//-----------------------------------------------------------------------------
SharedData* Client::_GetData() const {
    return reinterpret_cast<SharedData*>(mp_data);
}