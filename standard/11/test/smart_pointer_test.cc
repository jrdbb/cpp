#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <smart_pointer/unique_pointer.hpp>

namespace cpp {
namespace std11 {
namespace test {
using namespace testing;

class TestObject {
   public:
    TestObject(int i) : mValue(i) {}

    MOCK_METHOD(void, Die, ());
    ~TestObject() { Die(); }

    int mValue;
};

class SmartPointerTest : public Test {};

TEST_F(SmartPointerTest, Initialize) {
    {
        unique_ptr<TestObject> t(99);
        EXPECT_EQ(t->mValue, 99);
        EXPECT_CALL(*t, Die()).Times(1);
    }
}

TEST_F(SmartPointerTest, Reset) {
    {
        unique_ptr<TestObject> t(99);
        EXPECT_EQ(t->mValue, 99);
        EXPECT_CALL(*t, Die()).Times(1);
        t.reset(new TestObject(108));
        EXPECT_EQ(t->mValue, 108);
        EXPECT_CALL(*t, Die()).Times(1);
    }
}

TEST_F(SmartPointerTest, Move) {
    {
        unique_ptr<TestObject> t1(99);

        unique_ptr<TestObject> t2(199);

        EXPECT_CALL(*t2, Die()).Times(1);
        t2 = std::move(t1);
    }

    {
        unique_ptr<TestObject> t1(99);
        EXPECT_CALL(*t1, Die()).Times(1);

        unique_ptr<TestObject> t2(std::move(t1));
    }
}

}  // namespace test
}  // namespace std11
}  // namespace cpp
