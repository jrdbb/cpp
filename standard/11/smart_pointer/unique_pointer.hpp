#pragma once

#include <utility>

namespace cpp {
namespace std11 {

template <typename T>
class unique_ptr {
   public:
    unique_ptr() = default;
    explicit unique_ptr(T*);
    unique_ptr(unique_ptr<T>&& other);
    unique_ptr(const unique_ptr<T>&& other) = delete;

    unique_ptr(unique_ptr<T>& other) = delete;
    unique_ptr(const unique_ptr<T>& other) = delete;

    ~unique_ptr();

    void reset(T*);

    T& operator*();
    T& operator*() const;
    T* operator->();
    const T* operator->() const;
    unique_ptr<T>& operator=(unique_ptr<T>&& other);

   private:
    T* mValue = nullptr;
};

template <typename T>
unique_ptr<T>::unique_ptr(T* value) {
    mValue = value;
}

template <typename T>
unique_ptr<T>::unique_ptr(unique_ptr<T>&& other) {
    reset(other.mValue);
    other.mValue = nullptr;
}

template <typename T>
unique_ptr<T>::~unique_ptr() {
    if (mValue) {
        delete mValue;
    }
}

template <typename T>
void unique_ptr<T>::reset(T* value) {
    if (mValue) {
        delete mValue;
    }
    mValue = value;
}

template <typename T>
T& unique_ptr<T>::operator*() {
    return *mValue;
}

template <typename T>
T& unique_ptr<T>::operator*() const {
    return *mValue;
}

template <typename T>
T* unique_ptr<T>::operator->() {
    return mValue;
}

template <typename T>
const T* unique_ptr<T>::operator->() const {
    return mValue;
}

template <typename T>
unique_ptr<T>& unique_ptr<T>::operator=(unique_ptr<T>&& other) {
    reset(other.mValue);
    other.mValue = nullptr;
    return *this;
}

template <typename T, typename... Arg>
unique_ptr<T> make_unique(Arg&&... args)
{
    T* t = new T(std::forward<Arg>(args)...);
    return unique_ptr<T>(t);
}

}  // namespace std11
}  // namespace cpp