#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <declaration/function.hpp>
#include <regex>
#include <string>

namespace cpp::common::test {
using namespace testing;

// ref
struct TestObject {
    void refFunc() & { OnLValue(); }
    void refFunc() && { OnRValue(); }

    MOCK_METHOD(void, OnLValue, ());
    MOCK_METHOD(void, OnRValue, ());
};

class DeclarationTest : public Test {
   public:
    DeclarationTest() = default;

    TestObject mTestObj;
};

TEST_F(DeclarationTest, RefSpecifier) {
    EXPECT_CALL(mTestObj, OnLValue());
    EXPECT_CALL(mTestObj, OnRValue()).Times(0);

    mTestObj.refFunc();

    EXPECT_CALL(mTestObj, OnLValue()).Times(0);
    EXPECT_CALL(mTestObj, OnRValue());

    std::move(mTestObj).refFunc();
}

}  // namespace cpp::common::test
