#pragma once

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <string>

// Generator
#include <coroutine>
#include <fstream>
#include <utility>
#include <exception>
#include <random>
#include <optional>

template <typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type // required
    {
        //std::pair<int, int> current_value;
        T current_value;
        std::exception_ptr exception;

        //std::suspend_always yield_value(std::pair<int, int> value)
        std::suspend_always yield_value(T value)
        {
            current_value = value; // caching the result in promise
            return {};
        }
        
        Generator get_return_object()
        {
            return Generator{ this };
        }

        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void unhandled_exception() { exception = std::current_exception(); } // saving exception
        void return_void() {}
    };

    explicit Generator(promise_type* ip_promise) : h_coro(handle_type::from_promise(*ip_promise)) {}
    ~Generator() { if (h_coro) h_coro.destroy(); }

    bool next() const {
        if (!h_coro || h_coro.done())
            return false;

        h_coro.resume();
        return !h_coro.done();
    }

    T value() const { return h_coro.promise().current_value; }

private:
    handle_type h_coro;
};

//-----------------------------------------------------------------------------
class RandomPairGenerator {

    using TGenerator = Generator<std::pair<int, int>>;

public:
    explicit RandomPairGenerator(const std::optional<std::wstring>& i_file_name = std::nullopt);

    TGenerator Generate();

private:
    bool file_mode;
    std::ifstream file;
    std::random_device rd; // a seed source for the random number engine
    std::mt19937 gen;      // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> distrib;
};