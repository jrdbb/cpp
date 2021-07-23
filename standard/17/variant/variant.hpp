#pragma once
#include <algorithm>

namespace cpp::std17 {

namespace {

template <typename T, typename... Ts>
struct variant_traits {
   public:
    template <typename Tx>
    static constexpr bool contains = (std::is_same_v<Tx, T> ||
                                      (std::is_same_v<Tx, Ts> || ...));

    inline static void destroy(size_t id, void* data) {
        if (id == typeid(T).hash_code())
            reinterpret_cast<T*>(data)->~T();
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::destroy(id, data);
    }

    inline static void move(size_t old_t, void* old_v, void* new_v) {
        if (old_t == typeid(T).hash_code())
            new (new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::move(old_t, old_v, new_v);
    }

    inline static void copy(size_t old_t, const void* old_v, void* new_v) {
        if (old_t == typeid(T).hash_code())
            new (new_v) T(*reinterpret_cast<const T*>(old_v));
        else if constexpr (sizeof...(Ts) == 0)
            throw std::runtime_error("Unexpected");
        else
            variant_traits<Ts...>::copy(old_t, old_v, new_v);
    }
};

}  // namespace

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

    template <typename T,
              typename = std::enable_if_t<_traits::template contains<T>>>
    variant& operator=(const T& t) {
        if (mTypeID != typeid(T).hash_code()) {
            try_destory();
        }
        *(T*)(&mData) = t;
        return *this;
    }

   private:
    void try_destory() {
        if (mTypeID != 0) {
            _traits::destroy(mTypeID, &mData);
            mTypeID = 0;
        }
    }
    static const size_t data_size = std::max({sizeof(T_0), sizeof(Ts)...});
    static const size_t data_align = std::max({alignof(T_0), alignof(Ts)...});

    using data_t = typename std::aligned_storage<data_size, data_align>::type;

    void reset() { mTypeID = 0; }

    size_t mTypeID = 0;
    data_t mData;
};

}  // namespace cpp::std17