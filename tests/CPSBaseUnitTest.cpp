#include "CPSBase.hpp"
#include <gtest/gtest.h>

template <typename Func>
struct TestFunctor1 {
    template <typename... Args>
    int operator()(Args&&... args)
    {
        return Func {}(std::forward<Args>(args)...);
    }
};

using namespace Krypton;

TEST(CPSBaseUnitTest, GetParaTest1)
{
    using Func1 = TestFunctor1<GetKthArgument<0>>;
    using Func2 = TestFunctor1<GetKthArgument<2>>;
    ASSERT_EQ(Func1 {}(1, 2, 3, 4, 5, 6), 1);
    ASSERT_EQ(Func2 {}(1, 2, 3), 3);
}

TEST(CPSBaseUnitTest, FunctorTest1)
{
    int x = 1;
    using Func1 = TestFunctor1<LambdaBinder<GetKthArgument<0>>>;
    auto f = [&]() { return x; };
    ASSERT_EQ(Func1 {}(f), 1);
    x = 10;
    ASSERT_EQ(Func1 {}(f), 10);
    auto f2 = std::bind(Func1 {}, f);
    ASSERT_EQ(f2(), x);
    auto t = [&](int c) { return x + c; };
    using Func2 = TestFunctor1<LambdaBinder<GetKthArgument<0>, GetKthArgument<1>>>;
    auto f3 = std::bind(Func2 {}, t, std::placeholders::_1);
    ASSERT_EQ(f3(4), x + 4);
}
