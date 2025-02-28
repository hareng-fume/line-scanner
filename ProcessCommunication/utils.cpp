// ProcessCommunication.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "utils.h"

#include <cassert>
#include <chrono>
#include <cwctype>
#include <iostream>

#include <Windows.h>

namespace utils {

    
    /*
        args: e.g. a.exe --time-limit=500ns
                   a.exe --time-limit=500ns --threshold=100
                   a.exe --time-limit=500ns --threshold=100 --csv-test="C:\..."
    */

    //-----------------------------------------------------------------------------
    std::unordered_map<std::wstring, std::wstring> _parse_args(int argc, wchar_t* argv[]) {
        std::unordered_map<std::wstring, std::wstring> args;
        for (int i = 1; i < argc; ++i) {
            std::wstring_view arg = argv[i];

            // e.g. --threshold=100 -> 't'
            auto first = arg.find_first_not_of(L'-');
            assert(first != arg.npos);

            // e.g. --threshold=100
            if (auto last = arg.find(L'='); last != arg.npos) {
                std::wstring key(arg.substr(first, last-first));
                std::wstring val(arg.substr(last + 1));

                assert(args.count(key) == 0);
                args.insert({std::move(key), std::move(val)});
            }
        }

        return args;
    }

    //-----------------------------------------------------------------------------
    std::chrono::nanoseconds _parse_duration(const std::wstring& wstr) {

        auto i = wstr.size() - 1;
        while (i >= 0 && !std::iswdigit(wstr[i]))
            --i;

        if (i < 0)
            throw std::invalid_argument("Invalid duration string: no numeric value found.");

        auto value = static_cast<int>(std::stod(wstr.substr(0, i+1)));
        auto unit = wstr.substr(i+1);
        if (unit == L"ns")
            return std::chrono::nanoseconds(value);
        if (unit == L"us")
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(value));

        throw std::invalid_argument("Unsupported time unit: " + utils::_to_astring(unit));
    }

    //-------------------------------------------------------------------------
    std::wstring _to_wstring(const std::string& a_string) {
        // Get the size required for the wide-character string (including null-terminator)
        auto size = MultiByteToWideChar(CP_UTF8, 0, a_string.c_str(), -1, nullptr, 0);
        std::wstring w_string(size, 0);

        MultiByteToWideChar(CP_UTF8, 0, a_string.c_str(), -1, &w_string[0], size);
        return w_string;
    }

    //-----------------------------------------------------------------------------
    std::string _to_astring(const std::wstring& wstr) {
        auto size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string a_string(size, 0);

        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &a_string[0], size, nullptr, nullptr);
        return a_string;
    }

} // namespace utils

