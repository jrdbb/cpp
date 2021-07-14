#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container/vector.hpp>
#include <vector>

namespace cpp::common::test {
using namespace testing;

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
    TestObject(int i) : mValue(i), mStub(*stub) { mStub.IntParamConstructor(); }
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
    void SetUp() override { stub.reset(new Stub()); }
    void TearDown() override { stub.reset(); }
};

using VectorTypes =
    ::testing::Types<container::vector<TestObject>, std::vector<TestObject>>;

TYPED_TEST_SUITE(VectorTest, VectorTypes);

template <typename T>
class VectorIntTest : public Test {};

using VectorIntTypes =
    ::testing::Types<container::vector<int>, std::vector<int>>;

TYPED_TEST_SUITE(VectorIntTest, VectorIntTypes);
}  // namespace
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

TYPED_TEST(VectorTest, MoveInit) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(5);
        TypeParam vec(5);
        size_t oldCap = vec.capacity();
        TypeParam moveVec(std::move(vec));

        EXPECT_EQ(moveVec.size(), 5);
        EXPECT_EQ(moveVec.capacity(), oldCap);
        for (size_t i = 0; i < moveVec.size(); ++i) {
            EXPECT_EQ(moveVec[i].mValue, 0);
        }

        EXPECT_TRUE(vec.empty());
        EXPECT_CALL(*stub, Die()).Times(5);
    }
}

TYPED_TEST(VectorTest, Resize) {
    {
        EXPECT_CALL(*stub, NoParamConstructor()).Times(30);
        TypeParam vecToSmaller(10), vecToLarger(10), vecChangeCap(10);

        EXPECT_CALL(*stub, Die()).Times(5);
        vecToSmaller.resize(5);

        EXPECT_CALL(*stub, Die()).Times(5);
        for (int i = 0; i < 5; ++i) {
            vecToLarger.pop_back();
        }

        EXPECT_EQ(vecToLarger.size(), 5);
        EXPECT_GE(vecToLarger.capacity(), 10);
        EXPECT_CALL(*stub, NoParamConstructor()).Times(5);
        vecToLarger.resize(10);

        EXPECT_CALL(*stub, Die()).Times(10);
        EXPECT_CALL(*stub, NoParamConstructor()).Times(10);
        EXPECT_CALL(*stub, CopyConstructor()).Times(10);
        vecChangeCap.resize(20);
        EXPECT_EQ(vecChangeCap.size(), 20);

        EXPECT_CALL(*stub, Die()).Times(35);
    }
}

TYPED_TEST(VectorTest, ParamResize) {
    {
        EXPECT_CALL(*stub, IntParamConstructor()).Times(1);
        TestObject testObj(1);
        EXPECT_CALL(*stub, NoParamConstructor()).Times(10);
        TypeParam vecToLarger(10);

        EXPECT_CALL(*stub, Die()).Times(AtLeast(5));
        for (int i = 0; i < 5; ++i) {
            vecToLarger.pop_back();
        }

        EXPECT_EQ(vecToLarger.size(), 5);
        EXPECT_GE(vecToLarger.capacity(), 10);
        // for std::vector<T> resize() has temporary value
        EXPECT_CALL(*stub, CopyConstructor()).Times(AtLeast(5));
        vecToLarger.resize(10, testObj);
        EXPECT_EQ(vecToLarger.size(), 10);
        EXPECT_EQ(vecToLarger.capacity(), 10);

        EXPECT_CALL(*stub, Die()).Times(11);
    }
}

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

        EXPECT_CALL(*stub, Die()).Times(98);
        for (size_t i = 0; i < 49; ++i) {
            vec1.pop_back();
            vec2.pop_back();
        }
        EXPECT_CALL(*stub, CopyConstructor()).Times(1);
        EXPECT_CALL(*stub, Die()).Times(1);
        vec1.shrink_to_fit();
        EXPECT_EQ(vec1, vec2);
        EXPECT_EQ(vec1.capacity(), 1);
        EXPECT_CALL(*stub, Die()).Times(2);
    }
}

TYPED_TEST(VectorTest, FrontAndBack) {
    {
        /*
         * calling front&back with empty vector will cause undefined behavior
         * so no empty test here
         */
        EXPECT_CALL(*stub, NoParamConstructor).Times(10);
        TypeParam nonConstVec(10);

        for (int i = 0; i < nonConstVec.size(); ++i) {
            nonConstVec[i].mValue = i;
        }
        EXPECT_EQ(nonConstVec.front().mValue, 0);
        EXPECT_EQ(nonConstVec.back().mValue, 9);

        EXPECT_CALL(*stub, CopyConstructor()).Times(10);
        const TypeParam constVec{nonConstVec};
        EXPECT_EQ(constVec.front().mValue, 0);
        EXPECT_EQ(constVec.back().mValue, 9);

        EXPECT_CALL(*stub, Die()).Times(20);
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

TYPED_TEST(VectorTest, TraverseIterator) {
    {
        EXPECT_CALL(*stub, IntParamConstructor()).Times(1);
        EXPECT_CALL(*stub, CopyConstructor()).Times(5);
        EXPECT_CALL(*stub, Die()).Times(1);
        TypeParam vec(5, 1);
        for (auto& it : vec) {
            EXPECT_EQ(it.mValue, 1);
        }
        EXPECT_CALL(*stub, Die()).Times(5);
    }
}

TYPED_TEST(VectorIntTest, IteratorDereferencePlus) {
    TypeParam vec(5, 1);
    auto iter = vec.begin();
    *iter++;
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 1);
    EXPECT_NE(iter, vec.begin());
}

TYPED_TEST(VectorIntTest, IteratorSwap) {
    TypeParam vec(2);
    vec[0] = 1;
    auto it1 = vec.begin();
    auto it2 = it1 + 1;
    swap(it1, it2);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 0);
    EXPECT_EQ(*it2, 1);
    EXPECT_EQ(*it1, 0);
}

TYPED_TEST(VectorIntTest, IteratorSort) {
    TypeParam vec{3, 4, 1, 2, 5};
    std::sort(vec.begin(), vec.end());
    EXPECT_THAT(vec, ElementsAre(1, 2, 3, 4, 5));
}
}  // namespace cpp::common::test