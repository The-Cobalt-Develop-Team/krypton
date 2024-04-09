#pragma once

#include <tuple>
#include <utility>

namespace Krypton {

namespace Detail {
    template <int Index, typename... Args>
    struct GetKthTemplateArgType {
        using type = typename std::tuple_element<Index, std::tuple<Args...>>::type;
    };
}

template <typename F, typename... Args>
using ReturnType = typename std::invoke_result<F(Args..)>::type;

template <int Index>
struct GetKthArgument {
    template <typename... Args>
    typename Detail::GetKthTemplateArgType<Index, Args...>::type operator()(Args&&... args)
    {
        auto t = std::forward_as_tuple(std::forward<Args>(args)...);
        return std::get<Index>(t);
    }
};

template <typename PrevFunctor, typename... LambdaPara>
struct LambdaBinder {
    template <typename... Args>
    auto operator()(Args&&... args)
    {
        auto f = PrevFunctor {}(std::forward<Args>(args)...);
        return f(LambdaPara {}(std::forward<Args>(args)...)...);
    }
};

}
