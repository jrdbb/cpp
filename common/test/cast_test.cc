#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

namespace cpp::common::test {
using namespace testing;

namespace {

class TestObject {
   public:
    const int i = 0;
    void Set(int v) { const_cast<int&>(i) = 2; }
};

class CastTest : public Test {
   public:
    CastTest() = default;
};
}  // namespace

TEST_F(CastTest, ConstCastOnClassMember) {
    TestObject t;
    t.Set(2);
    EXPECT_EQ(t.i, 2);
}

}  // namespace cpp::common::test
