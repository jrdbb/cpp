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
    TestObject(const TestObject&) {}
    MOCK_METHOD(void, Die, ());
    ~TestObject() { Die(); }
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

}  // namespace cpp::common::test