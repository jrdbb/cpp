#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container/bvector.hpp>
#include <vector>

namespace cpp::common::test {
using namespace testing;

template <typename T>
class VectorBoolTest : public Test {
   protected:
};

using VectorTypes =
    ::testing::Types<container::vector<bool>, std::vector<bool>>;

TYPED_TEST_SUITE(VectorBoolTest, VectorTypes);

TYPED_TEST(VectorBoolTest, Bool) {
    {
        TypeParam vec(5, true);
        for (const auto v : vec) {
            EXPECT_EQ(v, true);
        }
    }
}

}  // namespace cpp::common::test