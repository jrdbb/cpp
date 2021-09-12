#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <reflection/reflection.hpp>
#include <string>

namespace cpp::common::test {
namespace {

class TestObject {
   public:
    int i = 0;
};
}  // namespace
}  // namespace cpp::common::test

// Reflection Def
namespace cpp::common::reflection {
template <>
struct ObjectDef<test::TestObject> {
    ObjectDef() = delete;
    struct Name {
        static constexpr std::string_view View = "TestObject";
    };
    struct i {
        static constexpr std::string_view View = "i";
    };
    static constexpr ElemList Properties = {
        Property{i{}, &test::TestObject::i}};
};

}  // namespace cpp::common::reflection

namespace cpp::common::test {
using namespace testing;

namespace {

class ReflectionTest : public Test {
   public:
    ReflectionTest() = default;
};
}  // namespace

TEST_F(ReflectionTest, Init) {
    TestObject t;
    reflection::Object<TestObject> obj{&t};
    EXPECT_EQ(obj.GetName(), "TestObject");
    t.i = 1;
    EXPECT_EQ(obj.GetProperty<reflection::ObjectDef<test::TestObject>::i>(),
              t.i);

    obj.SetProperty<reflection::ObjectDef<test::TestObject>::i>(2);
    EXPECT_EQ(2, t.i);
}

}  // namespace cpp::common::test
