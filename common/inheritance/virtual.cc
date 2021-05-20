#include "virtual.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

namespace cpp::common::inheritance {

Integer::Integer(int i) : mValue(i) {
    std::cout << "init integer " << mValue << std::endl;
    mFlag = 'i';
};

Integer::Integer(double d) {
    mValue = std::floor(d);
    std::cout << "init integer " << mValue << std::endl;
    mFlag = 'd';
};

NonPositiveInteger_V::NonPositiveInteger_V(int i) : Integer(i) {
    if (i > 0) {
        throw std::runtime_error("Initialize NonPositiveInteger with " +
                                 std::to_string(i));
    }
    mFlag = 'p';
}

NonNegativeInteger_V::NonNegativeInteger_V(int i) : Integer(i) {
    if (i < 0) {
        throw std::runtime_error("Initialize NonNegativeInteger with " +
                                 std::to_string(i));
    }
    mFlag = 'n';
}

NonPositiveInteger::NonPositiveInteger(int i) : Integer(i) {
    if (i > 0) {
        throw std::runtime_error("Initialize NonPositiveInteger with " +
                                 std::to_string(i));
    }
}

NonNegativeInteger::NonNegativeInteger(int i) : Integer(i) {
    if (i < 0) {
        throw std::runtime_error("Initialize NonNegativeInteger with " +
                                 std::to_string(i));
    }
}

ZeroInteger_V::ZeroInteger_V(int i)
    : Integer(double(i)), NonPositiveInteger_V(i), NonNegativeInteger_V(i) {}

ZeroInteger::ZeroInteger(int i)
    : NonNegativeInteger(i), NonPositiveInteger(i) {}

}  // namespace cpp::common::inheritance
