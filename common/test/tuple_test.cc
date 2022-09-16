#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <tuple>

namespace cpp::common::test {
using namespace testing;

namespace {

class TestObject {
   public:
    int i = 0;
};

class TupleTest : public Test {
   public:
    TupleTest() = default;
};
}  // namespace

TEST_F(TupleTest, Init) {
    auto t = std::make_tuple(1, 2, 3);
    auto b = std::is_same_v<std::tuple_element_t<0, decltype(t)>, int>;
    EXPECT_TRUE(b);
}

}  // namespace cpp::common::test
