#include "processor_client.h"

#include <utils.h>

#include <array>

using namespace std::chrono_literals;

namespace {

    static constexpr const std::array<double, 9> _FILTER = {
        0.00025177, 0.008666992, 0.078025818, 0.24130249, 0.343757629, 0.24130249, 0.078025818, 0.008666992, 0.000125885
    };

    // threshold hint:
    //      min averaged filtered value: 124.027804061529
    //      max averaged filtered value: 131.1727234194942
    static constexpr const double default_threshold = (131.1727234194942 + 124.027804061529) / 2;

} // namespace

int wmain(int argc, wchar_t* argv[]) {

    std::cout << "Processor started" << std::endl;

    auto args = utils::_parse_args(argc, argv);
    auto time_limit = args.contains(utils::TIME_LIMIT) ?
        utils::_parse_duration(args.at(utils::TIME_LIMIT)) : 500ns;
    auto threthold = args.contains(utils::THRESHOLD) ?
        std::stod(args.at(utils::THRESHOLD)) : default_threshold;

    ClientPairProcessor client(time_limit, threthold);
    if (!client.Init())
        return EXIT_FAILURE;

    client.Start();
    return EXIT_SUCCESS;
}
