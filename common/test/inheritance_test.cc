#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <inheritance/virtual.hpp>
#include <regex>
#include <string>

namespace cpp::common::test {
using namespace testing;

class InheritanceTest : public Test {
   public:
    InheritanceTest() = default;

    MOCK_METHOD(void, OnIntegerConstructor, (int));

    void CaptureStdout() { testing::internal::CaptureStdout(); }

    void Flush() {
        std::string output = testing::internal::GetCapturedStdout();
        std::stringstream ss;
        ss << output;
        std::string line;
        while (std::getline(ss, line).good()) {
            std::smatch match;
            if (std::regex_search(output, match,
                                  std::regex(R"reg(init integer (\d+))reg"))) {
                OnIntegerConstructor(std::stoi(match[1]));
            }
        }
    }
};

TEST_F(InheritanceTest, HowManyTimesIsConstructorInvoked) {
    CaptureStdout();
    EXPECT_CALL(*this, OnIntegerConstructor(_)).Times(2);
    inheritance::ZeroInteger i(0);
    Flush();

    CaptureStdout();
    EXPECT_CALL(*this, OnIntegerConstructor(_)).Times(1);
    inheritance::ZeroInteger_V i_v(0);
    Flush();
    EXPECT_EQ(i_v.mFlag, 'p');  // in order in inheritance declaration
}

}  // namespace cpp::common::test
