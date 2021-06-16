#pragma once

#include <utility>

namespace cpp {
namespace std11 {

// helper type
class _Undefined_class;

union _Nocopy_types {
    void* _M_object;
    const void* _M_const_object;
    void (*_M_function_pointer)();
    void (_Undefined_class::*_M_member_pointer)();
};

union _Any_data {
    void* _M_access() { return &_M_pod_data[0]; }
    const void* _M_access() const { return &_M_pod_data[0]; }

    template <typename _Tp>
    _Tp& _M_access() {
        return *static_cast<_Tp*>(_M_access());
    }

    template <typename _Tp>
    const _Tp& _M_access() const {
        return *static_cast<const _Tp*>(_M_access());
    }

    _Nocopy_types _M_unused;
    char _M_pod_data[sizeof(_Nocopy_types)];
};

static void _M_init_functor(_Any_data& __functor, _Functor&& __f) {
    _M_init_functor(__functor, std::move(__f));
}

template <typename R, typename... Arg>
class function;

// specialization
template <typename R, typename... Arg>
class function<R(Arg...)> {
   public:
    using Pfunc = R (*)(Arg...);
    R operator()(Arg... arg) { return pfunc_(arg...); }

   private:
    Pfunc pfunc_;
};
}  // namespace std11
}  // namespace cpp