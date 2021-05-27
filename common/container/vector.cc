#include "vector.hpp"

namespace cpp::common::container {

static size_t calculate_storage(size_t size) {
    /*
     * if on x86
     * asm(bsr)
     */
    size_t storage = 1;
    while (storage < size) {
        storage = storage << 1;
    }
    return storage;
}

template <typename T>
vector<T>::vector(size_t n) {
    mvector_data.begin = new T[n];
    mvector_data.storage = n;
}

template <typename T>
vector<T>::vector(size_t n, const T& val) {
    size_t new_storage = calculate_storage(n);
    mvector_data.begin = new T[new_storage](val);

    mvector_data.used = n;
    mvector_data.storage = new_storage;
}

template <typename T>
vector<T>::vector(const vector<T>& rhs) {
    *this = rhs;
}

template <typename T>
vector<T>::vector(vector<T>&& rhs) {
    *this = rhs;
}

template <typename T>
vector<T>::~vector() {}

template <typename T>
size_t vector<T>::size() const noexcept {
    return mvector_data.used;
}

template <typename T>
vector<T>& vector<T>::operator=(const vector<T>& rhs) {
    mvector_data.begin = new T[rhs.mvector_data.storage];
    for (size_t index = 0; index < mvector_data.used; ++index) {
        mvector_data.begin[index] = rhs.mvector_data.begin[index];
    }
    mvector_data.used = rhs.mvector_data.used;
    mvector_data.storage = rhs.mvector_data.storage;
}

template <typename T>
vector<T>& vector<T>::operator=(vector<T>&& rhs) {
    mvector_data.begin = rhs.begin;
    rhs.begin = nullptr;

    mvector_data.used = rhs.mvector_data.used;
    rhs.mvector_data.used = 0;
    mvector_data.storage = rhs.mvector_data.storage;
    rhs.mvector_data.storage = 0;
}

template <typename T>
void vector<T>::resize(size_t n) {
    reserve(n);
    mvector_data.used = n;
}

template <typename T>
void vector<T>::resize(size_t n, const T& val) {
    size_t used = mvector_data.used;
    resize(n);
    for (size_t index = used; index < n; ++index) {
        mvector_data[index] = val;
    }
}

template <typename T>
size_t vector<T>::capacity() const noexcept {
    return mvector_data.storage;
}

template <typename T>
bool vector<T>::empty() const noexcept {
    return mvector_data.used == 0;
}

template <typename T>
void vector<T>::reserve(size_t n) {
    if (mvector_data.storage < n) {
        size_t new_storage = calculate_storage(n);
        T* temp = new T[new_storage];
        for (size_t index = 0; index < mvector_data.used; ++index) {
            temp[index] = mvector_data.begin[index];
        }

        delete[] mvector_data.begin;
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
void vector<T>::shrink_to_fit() {
    size_t new_storage = calculate_storage(mvector_data.used);
    if (new_storage != mvector_data.storage) {
        T* temp = new T[new_storage];

        for (size_t index = 0; index < mvector_data.used; ++index) {
            temp[index] = mvector_data.begin[index];
        }

        delete[] mvector_data.begin;
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}
}  // namespace cpp::common::container