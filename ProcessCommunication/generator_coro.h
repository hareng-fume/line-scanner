#pragma once

#include <coroutine>
#include <exception>

template <typename T>
struct Generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;

    struct promise_type // required
    {
        T current_value;
        std::exception_ptr exception;

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
