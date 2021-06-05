#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container/vector.hpp>
#include <vector>

namespace cpp::common::test {
using namespace testing;

struct Stub {
    MOCK_METHOD(void, NoParamConstructor, ());
    MOCK_METHOD(void, IntParamConstructor, ());
    MOCK_METHOD(void, CopyConstructor, ());
    MOCK_METHOD(void, MoveConstructor, ());
    MOCK_METHOD(void, Die, ());
};

static Stub stub;

class TestObject {
   public:
    TestObject() { mStub.NoParamConstructor(); }
    TestObject(int i) : mValue(i) { mStub.IntParamConstructor();}
    TestObject(const TestObject&) { mStub.CopyConstructor();}
    MOCK_METHOD(void, Die, ());
    ~TestObject() { Die(); mStub.Die(); }
    int mValue;
    Stub& mStub = stub;
};

template <typename T>
class VectorTest : public Test {
   public:
    T mDefaultVec;
};

using VectorTypes =
    ::testing::Types<container::vector<TestObject>, std::vector<TestObject>>;

TYPED_TEST_SUITE(VectorTest, VectorTypes);
TYPED_TEST(VectorTest, Resize) {
    {
        EXPECT_CALL(stub,No)
        EXPECT_CALL(stub,CopyConstructor()).Times(6);
        EXPECT_CALL(stub,CopyConstructor()).Times(6);

        for (int i = 0; i < 6; ++i) {
            this->mDefaultVec.push_back(TestObject());
        }
        EXPECT_EQ(this->mDefaultVec.capacity(), 8);
        auto* t = &this->mDefaultVec[5];
        EXPECT_CALL(*t, Die()).Times(1);
        this->mDefaultVec.resize(5);
        EXPECT_EQ(this->mDefaultVec.capacity(), 8);
    }
}

TYPED_TEST(VectorTest, Init) {
    TypeParam vec(5);
    EXPECT_EQ(vec.size(), 5);
    EXPECT_DOUBLE_EQ
    // this->InitTest(5);
    // EXPECT_EQ(this->vec.capacity(), 10);
    // std::vector<TestObject> stdvecNoParamInit(5);
    // std::cout << stdvecNoParamInit.capacity() << std::endl;

    // std::vector<TestObject> stdvecParamInit(5, 1);
    // std::cout << stdvecParamInit.capacity() << std::endl;

    // std::vector<TestObject> stdvecVecInit(stdvecParamInit);
    // std::cout << stdvecParamInit.capacity() << std::endl;
}

// TEST_F(VectorTest, FrontAndBack) {
//     std::vector<TestObject> stdvec;
//     auto& front_empty = stdvec.front();
//     auto& back_empty = stdvec.back();
//     // std::cout<<front_empty.mValue<<std::endl;
//     // std::cout<<back_empty.mValue<<std::endl;
//     stdvec.reserve(4);
//     auto& front = stdvec.front();
//     auto& back = stdvec.back();
//     std::cout << stdvec.size() << std::endl;
//     std::cout << stdvec[-1].mValue << std::endl;
//     std::cout << stdvec[0].mValue << std::endl;
//     std::cout << stdvec[1].mValue << std::endl;
//     std::cout << stdvec[2].mValue << std::endl;
//     std::cout << stdvec[3].mValue << std::endl;

//     std::cout << front.mValue << std::endl;
//     std::cout << back.mValue << std::endl;
// }

// TEST_F(VectorTest, PushBack) {
//     std::vector<TestObject> stdvec;
//     stdvec.resize(5);

//     stdvec.push_back(TestObject());
//     std::cout << stdvec.capacity() << std::endl;
//     stdvec.pop_back();
//     std::cout << stdvec.capacity() << std::endl;
// }

// TEST_F(VectorTest, PopBack) {
//     std::vector<TestObject> stdvec;
//     stdvec.reserve(1);
//     stdvec.pop_back();
//     // stdvec.pop_back();
// }

// TEST_F(VectorTest, Clear) {
//     std::vector<TestObject> stdvec(5);
//     EXPECT_EQ(stdvec.capacity(), 5);
//     stdvec.clear();
//     EXPECT_EQ(stdvec.capacity(), 5);
// }

}  // namespace cpp::common::test