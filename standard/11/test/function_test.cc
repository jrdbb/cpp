#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <function/function.hpp>
#include <functional>

namespace cpp {
namespace std11 {
namespace test {
using namespace testing;

class FunctionTest : public Test {};

TEST_F(FunctionTest, functionPointer) {
    // function<int, int> f;
    std::function<int(int)> f;
}

}  // namespace test
}  // namespace std11
}  // namespace cpp
