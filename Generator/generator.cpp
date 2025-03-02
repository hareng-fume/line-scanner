#include "generator.h"

#include <sstream>

//-----------------------------------------------------------------------------
UniformDistr::UniformDistr(int i_min, int i_max)
    : m_gen(std::random_device()()) // std::random_device is a seed source for the random number engine
    , m_distrib(i_min, i_max) {}

//-----------------------------------------------------------------------------
int UniformDistr::Draw() {
    return m_distrib(m_gen);
}

//-----------------------------------------------------------------------------
RandomPairGenerator::RandomPairGenerator(
    const std::optional<std::wstring>& i_file_name /*= std::nullopt*/)
    : UniformDistr(0, 255)
    , file_mode(i_file_name.has_value())
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
        int i = 0;
        while (true) {
            co_yield{ Draw(), Draw() };
            ++i;
        }
    }
}
