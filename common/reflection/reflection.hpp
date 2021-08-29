#pragma once

#include <string>
#include <string_view>
#include <tuple>

namespace cpp::common::reflection {

template <typename Name, typename T>
struct Property;
template <typename T>
struct ObjectDef;
template <typename T>
struct Object;

namespace detail {

template <typename List, typename Pred>
constexpr std::size_t FindIf(const List&, Pred&&, std::index_sequence<>) {
    return -1;
}
template <typename List, typename Pred, std::size_t N0, std::size_t... Ns>
constexpr std::size_t FindIf(const List& l, Pred&& p,
                             std::index_sequence<N0, Ns...>) {
    return p(std::get<N0>(l))
               ? N0
               : FindIf(l, std::forward<Pred>(p), std::index_sequence<Ns...>{});
}

template <typename Obj, typename Value>
struct PropertyTraitsBase;
template <typename T>
struct PropertyTraits;

template <typename Obj, typename Value>
struct PropertyTraitsBase {
    using object_type = Obj;
    using value_type = Value;
};

template <typename Object, typename T>
struct PropertyTraits<T Object::*> : PropertyTraitsBase<Object, T> {};

}  // namespace detail

template <typename... T>
struct ElemList {
    std::tuple<T...> mElems;
    constexpr ElemList(T... elems) : mElems{elems...} {}
    constexpr static size_t size = sizeof...(T);

    template <typename Name>
    constexpr decltype(auto) GetElemIndex() const {
        return detail::FindIf(
            mElems, [](auto e) { return decltype(e)::NameView == Name::View; },
            std::make_index_sequence<size>{});
    }

    template <std::size_t index>
    constexpr decltype(auto) GetElem() const {
        return std::get<index>(mElems);
    }
};

template <typename Name, typename T>
struct Property : detail::PropertyTraits<T> {
    constexpr Property(Name, T value) : value{value} {}
    constexpr static std::string_view NameView = Name::View;
    T value;
};

template <typename T>
struct ObjectDef {
    ObjectDef() = delete;
    // struct Name {
    //     static constexpr std::string_view View = nullptr;
    // }
    // static constexpr ElemList<> Properties;
};

template <typename T>
struct Object {
    Object(T* obj) : mObj(obj) {}

    constexpr const std::string_view GetName() {
        return ObjectDef<T>::Name::View;
    }

    template <typename Name>
    constexpr decltype(auto) GetProperty() const {
        constexpr auto index =
            ObjectDef<T>::Properties.template GetElemIndex<Name>();
        static_assert(index != -1);
        return mObj->*ObjectDef<T>::Properties.template GetElem<index>().value;
    }

    template <typename Name, typename V>
    constexpr void SetProperty(V&& value) {
        constexpr auto index =
            ObjectDef<T>::Properties.template GetElemIndex<Name>();
        static_assert(index != -1);
        mObj->*ObjectDef<T>::Properties.template GetElem<index>().value = value;
    }

   private:
    T* mObj;
};

}  // namespace cpp::common::reflection