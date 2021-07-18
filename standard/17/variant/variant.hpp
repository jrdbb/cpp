#pragma once
#include <algorithm>

namespace cpp::std17 {

namespace {
template <typename... Ts>
class variant_traits {
   public:
    template <typename T>
    static constexpr bool contains = (std::is_same_v<T, Ts> || ...);
};
}  // namespace

template <typename... Ts>
class variant {
   public:
    variant() = default;
    variant(const variant& __rhs) = default;
    variant(variant&&) = default;
    variant& operator=(const variant&) = default;
    variant& operator=(variant&&) = default;
    ~variant() = default;

    using _traits = variant_traits<Ts...>;

    template <typename T,
              typename = std::enable_if_t<_traits::template contains<T>>>
    variant(T&& t) {
        mTypeID = typeid(T).hash_code();
        new (&mData) T(std::forward<T>(t));
    }

   private:
    static const size_t data_size = std::max({size_t(0), sizeof(Ts)...});
    static const size_t data_align = std::max({size_t(0), alignof(Ts)...});

    using data_t = typename std::aligned_storage<data_size, data_align>::type;

    size_t mTypeID;
    data_t mData;
};

}  // namespace cpp::std17