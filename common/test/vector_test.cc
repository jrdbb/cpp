#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container/vector.hpp>
#include <vector>

namespace cpp::common::test {
using namespace testing;

class TestObject {
   public:
    MOCK_METHOD(void, NoParamConstructor, ());
    TestObject() { NoParamConstructor(); }
    TestObject(int i) : mValue(i) {}
    TestObject(const TestObject&) {}
    MOCK_METHOD(void, Die, ());
    ~TestObject() { Die(); }
    int mValue;
};

class VectorTest : public Test {};

TEST_F(VectorTest, Resize) {
    {
        std::vector<TestObject> stdvec;

        for (int i = 0; i < 6; ++i) {
            stdvec.emplace_back();
        }
        std::cout << stdvec.capacity() << std::endl;
        auto* t = &stdvec[5];
        EXPECT_CALL(*t, Die()).Times(1);
        stdvec.resize(5);
        std::cout << stdvec.capacity() << std::endl;
    }
}

TEST_F(VectorTest, Init) {
    std::vector<TestObject> stdvecNoParamInit(5);
    std::cout << stdvecNoParamInit.capacity() << std::endl;

    std::vector<TestObject> stdvecParamInit(5, 1);
    std::cout << stdvecParamInit.capacity() << std::endl;

    std::vector<TestObject> stdvecVecInit(stdvecParamInit);
    std::cout << stdvecParamInit.capacity() << std::endl;
}

TEST_F(VectorTest, FrontAndBack) {
    std::vector<TestObject> stdvec;
    auto& front_empty = stdvec.front();
    auto& back_empty = stdvec.back();
    // std::cout<<front_empty.mValue<<std::endl;
    // std::cout<<back_empty.mValue<<std::endl;
    stdvec.reserve(4);
    auto& front = stdvec.front();
    auto& back = stdvec.back();

    EXPECT_EQ(front.mValue, stdvec[0].mValue);
    EXPECT_EQ(back.mValue, stdvec[-1].mValue);
}

TEST_F(VectorTest, PushBack) {
    std::vector<TestObject> stdvec;
    stdvec.resize(4);

    stdvec.push_back(TestObject());
    EXPECT_EQ(stdvec.capacity(), 8);
    stdvec.pop_back();
    EXPECT_EQ(stdvec.capacity(), 8);
}

TEST_F(VectorTest, PopBack) {
    std::vector<TestObject> stdvec;
    stdvec.reserve(1);
    EXPECT_DEATH(stdvec.pop_back(), "");
}

TEST_F(VectorTest, Clear) {
    std::vector<TestObject> stdvec(5);
    EXPECT_EQ(stdvec.capacity(), 5);
    stdvec.clear();
    EXPECT_EQ(stdvec.capacity(), 5);
}
}  // namespace cpp::common::test