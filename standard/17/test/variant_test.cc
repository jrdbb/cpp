#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <variant/variant.hpp>
#include <variant>

namespace cpp::common::test {
using namespace testing;
using namespace cpp::std17;

namespace {
struct Stub {
    MOCK_METHOD(void, NoParamConstructor, ());
    MOCK_METHOD(void, IntParamConstructor, ());
    MOCK_METHOD(void, CopyConstructor, ());
    MOCK_METHOD(void, MoveConstructor, ());
    MOCK_METHOD(void, Die, ());
};

static std::unique_ptr<Stub> stub(nullptr);

class TestObject {
   public:
    TestObject() : mStub(*stub) { mStub.NoParamConstructor(); }
    explicit TestObject(int i) : mValue(i), mStub(*stub) { mStub.IntParamConstructor(); }
    TestObject(const TestObject& testObj)
        : mValue(testObj.mValue), mStub(testObj.mStub) {
        mStub.CopyConstructor();
    }
    TestObject(TestObject&& testObj) : mValue(testObj.mValue) {
        testObj.mValue = 0;
        mStub.MoveConstructor();
    }
    bool operator==(const TestObject& rhs) { return mValue == rhs.mValue; }
    bool operator!=(const TestObject& rhs) { return mValue != rhs.mValue; }
    TestObject& operator=(const TestObject& x) {
        mValue = x.mValue;
        return *this;
    }
    TestObject& operator=(TestObject&& x) {
        mValue = x.mValue;
        return *this;
    }
    ~TestObject() { mStub.Die(); }
    int mValue = 0;
    Stub& mStub = *stub;
};

template <typename T>
class VariantTest : public Test {
   protected:
    void SetUp() override { stub.reset(new Stub()); }
    void TearDown() override { stub.reset(); }
};

using VariantTypes =
    ::testing::Types<variant<TestObject, int>, std::variant<TestObject, int>>;

TYPED_TEST_SUITE(VariantTest, VariantTypes);
}  // namespace

TYPED_TEST(VariantTest, SizeTest) {
    EXPECT_EQ(sizeof(TypeParam), sizeof(TestObject) + sizeof(size_t));
}

TYPED_TEST(VariantTest, Init) {
    {
        EXPECT_CALL(*stub, NoParamConstructor());
        EXPECT_CALL(*stub, Die()).Times(2);
        TypeParam v(TestObject{});
    }
}

TYPED_TEST(VariantTest, Assignment) {
    {
        EXPECT_CALL(*stub, NoParamConstructor());
        EXPECT_CALL(*stub, Die()).Times(2);
        TypeParam v;

        v = TestObject(2);
    }
}

}  // namespace cpp::common::test