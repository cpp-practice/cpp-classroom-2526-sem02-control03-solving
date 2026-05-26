#pragma once
#include <tuple>


namespace control_03 {

    namespace details {
        template<class F>
        struct func_trait;

        template<class T, class ...Args>
        struct func_trait<T(*)(Args...)> {
            constexpr static std::size_t size = sizeof...(Args);
        };

        template<class T, class F, class ...Args>
        struct func_trait<T(F::*)(Args...)> {
            constexpr static std::size_t size = sizeof...(Args);
        };

        template<class T, class F, class ...Args>
        struct func_trait<T(F::*)(Args...) const> {
            constexpr static std::size_t size = sizeof...(Args);
        };

        template<class F>
        struct func_trait : func_trait<decltype(&F::operator())> {
        };


        template<class F, class ...Args>
        struct curry {
            F f;
            std::tuple<Args...> args = {};
            static  constexpr std::size_t limit = func_trait<F>::size;

            template<class ...Inps>
            constexpr decltype(auto) operator()(Inps &&...) requires (sizeof...(Inps) + sizeof...(Args) > limit) {
                static_assert(false);
            }

            template<class ...Inps>
            constexpr decltype(auto) operator()(Inps &&...inps) requires (sizeof...(Inps) + sizeof...(Args) == limit) {
                return std::apply(f, std::tuple_cat(std::move(args), std::make_tuple(std::forward<Inps>(inps)...)));
            }

            template<class ...Inps>
            constexpr decltype(auto) operator()(Inps &&...inps) requires (sizeof...(Inps) + sizeof...(Args) < limit) {
                return curry<F, Args..., std::decay_t<Inps>...>{std::move(f), std::tuple_cat(std::move(args), std::make_tuple(std::forward<Inps>(inps)...))};
            }
        };
    }

    template<class F>
    decltype(auto) curry(F &&f) {
        return details::curry{std::forward<F>(f)};
    }
}
