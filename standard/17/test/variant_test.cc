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
    explicit TestObject(int i) : mValue(i), mStub(*stub) {
        mStub.IntParamConstructor();
    }
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
    friend std::ostream& operator<<(std::ostream& os, const TestObject&);
    ~TestObject() { mStub.Die(); }
    int mValue = 0;
    Stub& mStub = *stub;
};

std::ostream& operator<<(std::ostream& os, const TestObject& t) {
    os << t.mValue;
    return os;
}

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
        // default ctor 1st type
        TypeParam v;

        v = TestObject(2);
    }
}

TYPED_TEST(VariantTest, Get) {
    {
        TypeParam v(2);

        EXPECT_TRUE((std::is_same_v<decltype(std::get<int>(v)), int&>));
        EXPECT_TRUE((std::is_same_v<decltype(std::get<1>(v)), int&>));

        EXPECT_EQ(std::get<int>(v), 2);
        EXPECT_EQ(std::get<1>(v), 2);

        try {
            std::get<TestObject>(v);
            FAIL() << "Expected exception";
        } catch (std::runtime_error const& err) {
        } catch (std::bad_variant_access const& err) {
        }

        // rvalue
        EXPECT_TRUE(
            (std::is_same_v<decltype(std::get<int>(std::move(v))), int&&>));
        EXPECT_TRUE(
            (std::is_same_v<decltype(std::get<1>(std::move(v))), int&&>));
    }

    {
        const TypeParam v(2);

        EXPECT_TRUE((std::is_same_v<decltype(std::get<int>(v)), const int&>));
        EXPECT_TRUE((std::is_same_v<decltype(std::get<1>(v)), const int&>));

        EXPECT_EQ(std::get<int>(v), 2);
        EXPECT_EQ(std::get<1>(v), 2);

        try {
            std::get<TestObject>(v);
            FAIL() << "Expected exception";
        } catch (std::runtime_error const& err) {
        } catch (std::bad_variant_access const& err) {
        }
    }
}

TYPED_TEST(VariantTest, Index) {
    {
        TypeParam v(2);
        EXPECT_EQ(v.index(), 1);
    }
}

// TYPED_TEST(VariantTest, Visit) {
//     {
//         TypeParam v(2);
//         std::visit([](auto&& arg) { std::cout << arg; }, v);
//     }
// }

}  // namespace cpp::common::test