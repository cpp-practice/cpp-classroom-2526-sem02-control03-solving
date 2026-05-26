#pragma once
#include <tuple>
#include <utility>
#include <functional>


namespace control_03 {
    template<class F, class ...Args>
    struct extension {
        F f;
        std::tuple<Args...> args;

        template<class I>
        constexpr decltype(auto) operator()(I &&x){
            return std::apply([&](auto &&...a) -> decltype(auto) {
                return f(std::forward<I>(x), std::forward<Args>(a)...);
            }, args);
        }
    };


    template<class T, class F, class ...Args>
    constexpr decltype(auto) operator|(T &&x, extension<F, Args...> &ex) {
        return ex(std::forward<T>(x));
    }
    template<class T, class F, class ...Args>
    constexpr decltype(auto) operator|(T &&x, extension<F, Args...> &&ex) {
        return std::move(ex)(std::forward<T>(x));
    }

    template<class F>
    constexpr decltype(auto) make_ext(F &&f) {
        return [f = std::forward<F>(f)]<class... Args>(Args &&...args) {
            return extension{f, std::make_tuple(std::forward<Args>(args)...)}; // also std::forward_as_tuple could be used to avoid decay
        };
    }
}
