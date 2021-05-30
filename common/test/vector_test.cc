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
    std::cout << stdvec.size() << std::endl;
    std::cout << stdvec[-1].mValue << std::endl;
    std::cout << stdvec[0].mValue << std::endl;
    std::cout << stdvec[1].mValue << std::endl;
    std::cout << stdvec[2].mValue << std::endl;
    std::cout << stdvec[3].mValue << std::endl;

    std::cout << front.mValue << std::endl;
    std::cout << back.mValue << std::endl;
}

TEST_F(VectorTest, PushBack) {
    std::vector<TestObject> stdvec;
    stdvec.resize(5);

    stdvec.push_back(TestObject());
    std::cout << stdvec.capacity() << std::endl;
    stdvec.pop_back();
    std::cout << stdvec.capacity() << std::endl;
}

TEST_F(VectorTest, PopBack) {
    std::vector<TestObject> stdvec;
    stdvec.reserve(1);
    EXPECT_DEATH(stdvec.pop_back());
}

}  // namespace cpp::common::test