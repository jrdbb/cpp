#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <smart_pointer/unique_pointer.hpp>

namespace cpp {
namespace std11 {
namespace test {
using namespace testing;

class TestObject {
   public:
    TestObject() = default;
    TestObject(int i) : mValue(i) {}

    void Add() { mValue++; }
    int Get() { return mValue; }
    MOCK_METHOD(void, Die, ());
    ~TestObject() { Die(); }

    int mValue;
};

class TestChild : public TestObject {
   public:
    TestChild() = default;
};

class UniquePointerTest : public Test {};

TEST_F(UniquePointerTest, Initialize) {
    {
        auto t = make_unique<TestObject>(99);
        EXPECT_EQ(t->mValue, 99);
        EXPECT_CALL(*t, Die()).Times(1);
    }
}

TEST_F(UniquePointerTest, Reset) {
    {
        auto t = make_unique<TestObject>(99);
        EXPECT_EQ(t->mValue, 99);
        EXPECT_CALL(*t, Die()).Times(1);
        t.reset(new TestObject(108));
        EXPECT_EQ(t->mValue, 108);
        EXPECT_CALL(*t, Die()).Times(1);
    }
}

TEST_F(UniquePointerTest, Move) {
    {
        auto t1 = make_unique<TestObject>(99);
        auto t2 = make_unique<TestObject>(199);

        EXPECT_CALL(*t2, Die()).Times(1);
        t2 = std::move(t1);
    }

    {
        auto t1 = make_unique<TestObject>(99);
        EXPECT_CALL(*t1, Die()).Times(1);

        unique_ptr<TestObject> t2(std::move(t1));
    }

    { unique_ptr<TestObject> t1((unique_ptr<TestObject>())); }
}

TEST_F(UniquePointerTest, ConstPointer) {
    {
        const TestObject* p = new TestObject(1);
        std::unique_ptr<const TestObject> up(p);
    }
    {
        const TestObject* const p = new TestObject(1);
        const std::unique_ptr<const TestObject> up(p);
    }
    {
        const TestObject* p = new TestObject(1);
        unique_ptr<const TestObject> my_up(p);
    }
    {
        const TestObject* const p = new TestObject(1);
        const unique_ptr<const TestObject> my_up(p);
        // invalid
        // my_up->Add();
        // auto my_up2 = std::move(my_up);
    }
}

TEST_F(UniquePointerTest, Conversion) {
    auto p = make_unique<TestChild>();
    auto f = [](unique_ptr<TestObject> p) {};
    f(std::move(p));
}

}  // namespace test
}  // namespace std11
}  // namespace cpp
