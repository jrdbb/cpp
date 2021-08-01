#pragma once
#include <algorithm>

namespace {

template <typename T, typename... Ts>
struct variant_traits {
   public:
    template <typename Tx>
    static constexpr bool contains = (std::is_same_v<Tx, T> ||
                                      (std::is_same_v<Tx, Ts> || ...));

    inline static void destroy(std::size_t id, void* data) {
        if (id == typeid(T).hash_code())
            reinterpret_cast<T*>(data)->~T();
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::destroy(id, data);
    }

    inline static void move(std::size_t old_t, void* old_v, void* new_v) {
        if (old_t == typeid(T).hash_code())
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::move(old_t, old_v, new_v);
    }

    inline static void copy(std::size_t old_t, const void* old_v, void* new_v) {
        if (old_t == typeid(T).hash_code())
            new (new_v) T(*reinterpret_cast<const T*>(old_v));
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::copy(old_t, old_v, new_v);
    }

    inline static std::size_t index(std::size_t id, std::size_t i = 0) {
        if (id == typeid(T).hash_code())
            return i;
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            return variant_traits<Ts...>::index(id, i + 1);
    }
};

template <std::size_t I, typename T, typename... Ts>
struct variant_index {
    using type = std::enable_if_t<sizeof...(Ts) >= I,
                                  typename variant_index<I - 1, Ts...>::type>;
};

template <typename T, typename... Ts>
struct variant_index<0, T, Ts...> {
    using type = T;
};

}  // namespace

namespace cpp::std17 {

template <typename T_0, typename... Ts>
class variant {
   public:
    template <typename Require =
                  std::enable_if_t<std::is_default_constructible_v<T_0>>>
    variant() {
        mTypeID = typeid(T_0).hash_code();
        new (&mData) T_0();
    };
    variant(const variant& rhs) { *this = rhs; };
    variant(variant&& rhs) { *this = std::move(rhs); };
    variant& operator=(const variant& rhs) {
        try_destory();
        mTypeID = rhs.mTypeID;
        if (mTypeID != 0) {
            _traits::copy(mTypeID, &rhs.mData, &mData);
        }
        return *this;
    };

    variant& operator=(variant&& rhs) {
        try_destory();
        mTypeID = rhs.mTypeID;
        if (mTypeID != 0) {
            _traits::move(mTypeID, &rhs.mData, &mData);
            rhs.mTypeID = 0;
        }
        return *this;
    };
    ~variant() { try_destory(); };

    using _traits = variant_traits<T_0, Ts...>;

    template <typename T,
              typename = std::enable_if_t<_traits::template contains<T>>>
    explicit variant(T&& t) {
        mTypeID = typeid(T).hash_code();
        new (&mData) T(std::forward<T>(t));
    }

    // observers
    constexpr std::size_t index() const {
        if (valueless_by_exception()) {
            return -1;
        }
        return _traits::index(mTypeID);
    }
    constexpr bool valueless_by_exception() const { return mTypeID == 0; }

    // assignment
    template <typename T,
              typename = std::enable_if_t<_traits::template contains<T>>>
    variant& operator=(const T& t) {
        if (mTypeID != typeid(T).hash_code()) {
            try_destory();
        }
        *(T*)(&mData) = t;
        return *this;
    }

    template <std::size_t I, class... Types>
    friend constexpr typename variant_index<I, Types...>::type& std::get(
        variant<Types...>& v);
    template <std::size_t I, class... Types>
    friend constexpr typename variant_index<I, Types...>::type&& std::get(
        variant<Types...>&& v);
    template <std::size_t I, class... Types>
    friend constexpr const typename variant_index<I, Types...>::type& std::get(
        const variant<Types...>& v);
    template <std::size_t I, class... Types>
    friend constexpr const typename variant_index<I, Types...>::type&& std::get(
        const variant<Types...>&& v);

    template <class T, class... Types>
    friend constexpr T& std::get(variant<Types...>& v);
    template <class T, class... Types>
    friend constexpr T&& std::get(variant<Types...>&& v);
    template <class T, class... Types>
    friend constexpr const T& std::get(const variant<Types...>& v);
    template <class T, class... Types>
    friend constexpr const T&& std::get(const variant<Types...>&& v);

   private:
    void try_destory() {
        if (mTypeID != 0) {
            _traits::destroy(mTypeID, &mData);
            mTypeID = 0;
        }
    }
    static const std::size_t data_size = std::max({sizeof(T_0), sizeof(Ts)...});
    static const std::size_t data_align =
        std::max({alignof(T_0), alignof(Ts)...});

    using data_t = typename std::aligned_storage<data_size, data_align>::type;

    void reset() { mTypeID = 0; }

    std::size_t mTypeID = 0;
    data_t mData;
};

}  // namespace cpp::std17

namespace std {

// variant size
template <class variant>
struct variant_size {};

template <class... Types>
struct variant_size<cpp::std17::variant<Types...>> {
    inline static constexpr std::size_t value = sizeof...(Types);
};

// get helper
template <class T, class... Types>
constexpr T& get(cpp::std17::variant<Types...>& v) {
    if (v.mTypeID == typeid(T).hash_code()) {
        return (T&)v.mData;
    }
    throw std::runtime_error("");
}

template <class T, class... Types>
constexpr T&& get(cpp::std17::variant<Types...>&& v) {
    if (v.mTypeID == typeid(T).hash_code()) {
        return (T &&) v.mData;
    }
    throw std::runtime_error("");
}

template <class T, class... Types>
constexpr const T& get(const cpp::std17::variant<Types...>& v) {
    if (v.mTypeID == typeid(T).hash_code()) {
        return (const T&&)v.mData;
    }
    throw std::runtime_error("");
}

template <class T, class... Types>
constexpr const T&& get(const cpp::std17::variant<Types...>&& v) {
    if (v.mTypeID == typeid(T).hash_code()) {
        return (const T&&)v.mData;
    }
    throw std::runtime_error("");
}

template <std::size_t I, class... Types>
constexpr typename variant_index<I, Types...>::type& get(
    cpp::std17::variant<Types...>& v) {
    return get<typename variant_index<I, Types...>::type>(v);
}

template <std::size_t I, class... Types>
constexpr typename variant_index<I, Types...>::type&& get(
    cpp::std17::variant<Types...>&& v) {
    return get<typename variant_index<I, Types...>::type>(std::move(v));
}

template <std::size_t I, class... Types>
constexpr const typename variant_index<I, Types...>::type& get(
    const cpp::std17::variant<Types...>& v) {
    return get<typename variant_index<I, Types...>::type>(v);
}

template <std::size_t I, class... Types>
constexpr const typename variant_index<I, Types...>::type&& get(
    const cpp::std17::variant<Types...>&& v) {
    return get<typename variant_index<I, Types...>::type>(std::move(v));
}

}  // namespace std