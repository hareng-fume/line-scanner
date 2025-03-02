#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <chrono>
#include <unordered_map>
#include <string>

namespace utils {

    // GENERATOR
    inline constexpr const wchar_t* CSV_TEST = L"csv-test";

    // COMMON
    inline constexpr const wchar_t* TIME_LIMIT = L"time-limit";

    // PROCESSOR
    inline constexpr const wchar_t* THRESHOLD = L"threshold";


    std::unordered_map<std::wstring, std::wstring> _parse_args(int argc, wchar_t* argv[]);
    std::chrono::nanoseconds _parse_duration(const std::wstring& wstr);

    std::wstring _to_wstring(const std::string& a_string);
    std::string _to_astring(const std::wstring& wstr);



    template <typename T, size_t SIZE>
    constexpr size_t get_size(T(&)[SIZE]) {
        return SIZE;
    }

} // namespace utils
