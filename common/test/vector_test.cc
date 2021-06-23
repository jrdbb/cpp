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

static std::unique_ptr<Stub> stub(new Stub);

class TestObject {
   public:
    TestObject() { mStub.NoParamConstructor(); }
    TestObject(int i) : mValue(i) { mStub.IntParamConstructor(); }
    TestObject(const TestObject& testObj) : mValue(testObj.mValue) {
        mStub.CopyConstructor();
    }
    TestObject(TestObject&& testObj) : mValue(testObj.mValue) {
        testObj.mValue = 0;
        mStub.MoveConstructor();
    }
    bool operator==(const TestObject& rhs) { return mValue == rhs.mValue; }
    bool operator!=(const TestObject& rhs) { return mValue != rhs.mValue; }
    // MOCK_METHOD(void, Die, ());
    ~TestObject() {
        // Die();
        mStub.Die();
    }
    int mValue = 0;
    Stub& mStub = *stub;
};

bool operator==(const TestObject& lhs, const TestObject& rhs) {
    return lhs.mValue == rhs.mValue;
}

bool operator!=(const TestObject& lhs, const TestObject& rhs) {
    return lhs.mValue != rhs.mValue;
}

template <typename T>
class VectorTest : public Test {
   protected:
    void SetUp() override {}
    void TearDown() override { stub.reset(new Stub); }
};

using VectorTypes =
    ::testing::Types<container::vector<TestObject>, std::vector<TestObject>>;

TYPED_TEST_SUITE(VectorTest, VectorTypes);

TYPED_TEST(VectorTest, SizeInit) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(5);
        TypeParam vec(5);
        EXPECT_EQ(vec.size(), 5);

        EXPECT_CALL(*stub, NoParamConstructor()).Times(1);
        TestObject testObj;
        for (size_t i = 0; i < vec.size(); ++i) {
            EXPECT_EQ(vec[i], testObj);
        }
        EXPECT_CALL(*stub, Die()).Times(6);
    }
}

TYPED_TEST(VectorTest, SizeAndValueInit) {
    {
        EXPECT_CALL(*stub, IntParamConstructor()).Times(1);
        TestObject testObj(5);
        EXPECT_CALL(*stub, CopyConstructor()).Times(5);
        TypeParam vec1(5, testObj);
        EXPECT_EQ(vec1.size(), 5);

        for (size_t i = 0; i < vec1.size(); ++i) {
            EXPECT_EQ(vec1[i], testObj);
        }
        EXPECT_CALL(*stub, Die()).Times(6);
    }
}

TYPED_TEST(VectorTest, CopyInit) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(5);
        TypeParam vec(5);

        for (size_t i = 0; i < vec.size(); ++i) {
            vec[i].mValue = i;
        }

        EXPECT_CALL(*stub, CopyConstructor()).Times(5);
        TypeParam copyVec(vec);
        EXPECT_EQ(copyVec, vec);
        EXPECT_CALL(*stub, Die()).Times(10);
    }
}

// TODO: MoveInit

TYPED_TEST(VectorTest, Reserve) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(10);
        TypeParam vecLarger(5), vecSmaller(5);
        int oldCapacity = vecLarger.capacity();

        EXPECT_CALL(*stub, CopyConstructor()).Times(5);
        EXPECT_CALL(*stub, Die()).Times(5);
        vecLarger.reserve(2 * oldCapacity);
        EXPECT_GE(vecLarger.capacity(), 2 * oldCapacity);
        EXPECT_EQ(vecLarger, vecSmaller);

        vecSmaller.reserve(0.5 * oldCapacity);
        EXPECT_EQ(vecSmaller.capacity(), oldCapacity);

        EXPECT_CALL(*stub, Die()).Times(10);
    }
}

TYPED_TEST(VectorTest, ShrinkToFit) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(100);
        TypeParam vec1(50), vec2(50);

        for (size_t i = 0; i < vec1.size() - 1; ++i) {
            vec1.pop_back();
            vec2.pop_back();
        }
        vec1.shrink_to_fit();
        EXPECT_EQ(vec1, vec2);
    }
}

TYPED_TEST(VectorTest, FrontAndBack) {
    {
        /*
         * calling front&back with empty vector will cause undefined behavior
         * so no empty test here
         */
        EXPECT_CALL(*stub, NoParamConstructor).Times(10);
        TypeParam testvec(10);

        for (int i = 0; i < testvec.size(); ++i) {
            testvec[i].mValue = i;
        }
        EXPECT_EQ(testvec.front().mValue, 0);
        EXPECT_EQ(testvec.back().mValue, 9);
        EXPECT_CALL(*stub, Die()).Times(10);
    }
}

TYPED_TEST(VectorTest, PushBack) {
    {
        TypeParam vec;
        EXPECT_CALL(*stub, IntParamConstructor()).Times(2);
        TestObject testObj1(1), testObj2(2);

        EXPECT_CALL(*stub, CopyConstructor()).Times(1);

        vec.push_back(testObj1);
        EXPECT_EQ(vec.size(), 1);
        EXPECT_EQ(vec[0], testObj1);

        // reverse then add to tail
        EXPECT_CALL(*stub, CopyConstructor()).Times(2);
        EXPECT_CALL(*stub, Die()).Times(1);
        vec.push_back(testObj2);
        EXPECT_EQ(vec.size(), 2);
        EXPECT_EQ(vec[1], testObj2);

        EXPECT_CALL(*stub, CopyConstructor()).Times(2);
        EXPECT_CALL(*stub, MoveConstructor()).Times(1);
        EXPECT_CALL(*stub, Die()).Times(2);
        vec.push_back(std::move(testObj2));
        EXPECT_EQ(vec.size(), 3);
        EXPECT_EQ(vec[2], vec[1]);
        EXPECT_EQ(testObj2.mValue, 0);

        EXPECT_CALL(*stub, Die()).Times(5);
    }
}

TYPED_TEST(VectorTest, PopBack) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(10);
        TypeParam vec(10);
        int capacity = vec.capacity();
        EXPECT_CALL(*stub, Die()).Times(1);
        vec.pop_back();
        EXPECT_EQ(vec.capacity(), capacity);
        EXPECT_EQ(vec.size(), 9);
        EXPECT_CALL(*stub, Die()).Times(9);
    }
}

TYPED_TEST(VectorTest, Clear) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(10);
        TypeParam vec(10);
        int capacity = vec.capacity();
        EXPECT_CALL(*stub, Die()).Times(10);
        vec.clear();
        EXPECT_EQ(vec.size(), 0);
        EXPECT_EQ(vec.capacity(), capacity);
        EXPECT_CALL(*stub, Die()).Times(0);
    }
}

TYPED_TEST(VectorTest, Swap) {
    {
        EXPECT_CALL(*stub, IntParamConstructor()).Times(2);
        EXPECT_CALL(*stub, CopyConstructor()).Times(15);
        TestObject testObj1(5), testObj2(10);
        TypeParam vec1(5, testObj1), vec2(10, testObj2);
        size_t oldCap1 = vec1.capacity(), oldCap2 = vec2.capacity();

        vec1.swap(vec2);

        EXPECT_EQ(vec1.size(), 10);
        EXPECT_EQ(vec2.size(), 5);
        EXPECT_EQ(vec1.capacity(), oldCap2);
        EXPECT_EQ(vec2.capacity(), oldCap1);
        for (size_t i = 0; i < vec1.size(); ++i) {
            EXPECT_EQ(vec1[i].mValue, testObj2.mValue);
        }
        for (size_t i = 0; i < vec2.size(); ++i) {
            EXPECT_EQ(vec2[i], testObj1);
        }

        EXPECT_CALL(*stub, Die()).Times(17);
    }
}

}  // namespace cpp::common::test