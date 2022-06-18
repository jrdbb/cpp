#include <utility>

namespace cpp::common::templates {

template <typename... Ts, typename TExpr>
constexpr auto is_valid(TExpr expr)
    -> decltype(expr(std::declval<Ts...>()), bool()) {
    return true;
}

template <typename...>
constexpr auto is_valid(...) -> bool {
    return false;
}

struct pos {
    int npos;
};

struct test {
    test() {
        static_assert(!is_valid<int>([](auto t) -> decltype((void)t.npos) {}));
        static_assert(is_valid<pos>([](auto t) -> decltype((void)t.npos) {}));
    }
};

}  // namespace cpp::common::templates