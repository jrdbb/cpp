namespace cpp {
namespace std {

template <typename T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant;
    constexpr operator value_type() const noexcept {
        return value;
    }  // custom type operator
};

using false_type = integral_constant<bool, false>;
using true_type = integral_constant<bool, true>;

template <typename T, typename U>
struct is_same : false_type {};

template <typename T>
struct is_same<T, T> : true_type {};

template <typename T>
struct is_array : false_type {};

template <typename T>
struct is_array<T[]> : true_type {};

template <typename T, ::std::size_t N>
struct is_array<T[N]> : true_type {};

template <typename T>
struct is_reference : false_type {};

template <typename T>
struct is_reference<T&> : true_type {};

template <typename T>
struct is_reference<T&&> : true_type {};

}  // namespace std
}  // namespace cpp