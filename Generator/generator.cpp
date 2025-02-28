#include "generator.h"

//-----------------------------------------------------------------------------
RandomPairGenerator::RandomPairGenerator(
    const std::optional<std::wstring>& i_file_name /*= std::nullopt*/)
    : file_mode(i_file_name.has_value())
    , rd()
    , gen(rd())
    , distrib(0, 255)
{
    if (file_mode)
        file.open(i_file_name.value());
}

//-----------------------------------------------------------------------------
RandomPairGenerator::TGenerator RandomPairGenerator::Generate() {
    if (file_mode && file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int x, y;
            char comma;
            if (ss >> x >> comma >> y)
                co_yield{ x, y };
        }
    }
    else {
        while (true) {
            co_yield{ distrib(gen), distrib(gen) };
        }
    }
}

