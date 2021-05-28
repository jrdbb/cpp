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

}  // namespace cpp::common::test