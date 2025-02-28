#include "generator_server.h"

#include <utils.h>

using namespace std::chrono_literals;

int wmain(int argc, wchar_t* argv[])
{
    auto args = utils::_parse_args(argc, argv);
    auto time_limit = args.contains(utils::TIME_LIMIT) ?
        utils::_parse_duration(args.at(utils::TIME_LIMIT)) : 500ns;
    auto csv_path = args.contains(utils::CSV_TEST) ?
        std::optional<std::wstring>{args.at(utils::CSV_TEST)} : std::nullopt;

    Server server(time_limit, csv_path);
    if (!server.Init())
        return EXIT_FAILURE;

    server.Start();
    return EXIT_SUCCESS;
}
