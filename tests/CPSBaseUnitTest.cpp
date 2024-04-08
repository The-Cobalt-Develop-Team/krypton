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