#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <type_traits/type_traits.hpp>

namespace cpp {
namespace std {
namespace test {
using namespace testing;

class TypeTraitsTest : public Test {};

TEST_F(TypeTraitsTest, IsSame) {
    EXPECT_EQ((is_same<int, int>()), true);
    EXPECT_EQ((is_same<int, float>()), false);
    EXPECT_EQ((is_same<int&, int&&>()), false);
}

}  // namespace test
}  // namespace std
}  // namespace cpp
