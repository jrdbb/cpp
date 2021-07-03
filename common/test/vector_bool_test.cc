#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <container/bvector.hpp>
#include <vector>

namespace cpp::common::test {
using namespace testing;

template <typename T>
class VectorBoolTest : public Test {
   protected:
};

using VectorTypes =
    ::testing::Types<container::vector<bool>, std::vector<bool>>;

TYPED_TEST_SUITE(VectorBoolTest, VectorTypes);

TYPED_TEST(VectorBoolTest, TrueInit) {
    {
        TypeParam vec(5, true);
        for (const auto v : vec) {
            EXPECT_EQ(v, true);
        }
    }
}

TYPED_TEST(VectorBoolTest, SizeInit) {
    TypeParam vec(5);
    for (const auto v : vec) {
        EXPECT_EQ(v, false);
    }
}

TYPED_TEST(VectorBoolTest, ShrinkToFit) {
    TypeParam vec1(100), vec2(100);

    for (size_t i = 0; i < 50; ++i) {
        vec1.pop_back();
        vec2.pop_back();
    }
    vec1.shrink_to_fit();
    EXPECT_EQ(vec1, vec2);
    EXPECT_LT(vec1.capacity(), vec2.capacity());
}
TYPED_TEST(VectorBoolTest, FrontAndBack) {
    TypeParam nonConstVec(10);
    for (int i = 0; i < nonConstVec.size(); ++i) {
        nonConstVec[i] = i % 2;
    }
    EXPECT_FALSE(nonConstVec.front());
    EXPECT_TRUE(nonConstVec.back());

    const TypeParam constVec{nonConstVec};
    EXPECT_FALSE(constVec.front());
    EXPECT_TRUE(constVec.back());
}

TYPED_TEST(VectorBoolTest, PushBack) {
    TypeParam vec;
    vec.push_back(true);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_TRUE(vec[0]);

    vec.push_back(false);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_FALSE(vec[1]);

    const int tobeadded = 10;
    for (int i = 0; i < tobeadded; ++i) {
        vec.push_back(i % 2);
    }
    EXPECT_EQ(vec.size(), 12);
    EXPECT_FALSE(vec[2]);
    EXPECT_TRUE(vec[11]);
}

TYPED_TEST(VectorBoolTest, PopBack) {
    TypeParam vec(10);
    int capacity = vec.capacity();
    vec.pop_back();
    EXPECT_EQ(vec.capacity(), capacity);
    EXPECT_EQ(vec.size(), 9);
    for (auto i : vec) {
        EXPECT_FALSE(i);
    }
}
}  // namespace cpp::common::test