#pragma once
#include <stdexcept>
#include <vector>

namespace cpp::common::container {

template <typename T>
class vector {
   public:
    vector() = default;
    explicit vector(size_t n);
    vector(size_t n, const T& val);
    vector(const vector<T>& x);
    vector(vector&&);
    // vector (initializer_list<value_type> il);

    ~vector();

    vector& operator=(const vector& x);
    vector& operator=(vector&& x);
    // vector& operator= (initializer_list<value_type> il);

    // capacity
    size_t size() const noexcept;
    /*
     * If n is smaller than the current container size, the content is
     * reduced to its first n elements, removing those beyond (and
     * destroying them).
     * If n is greater than the current container size, the content is
     * expanded by inserting at the end as many elements as needed to
     * reach a size of n. If val is specified, the new elements are
     * initialized as copies of val, otherwise, they are value-
     * initialized.
     * If n is also greater than the current container capacity, an
     * automatic reallocation of the allocated storage space takes
     * place.
     */
    void resize(size_t n);
    void resize(size_t n, const T& val);
    size_t capacity() const noexcept;
    bool empty() const noexcept;
    /*
     * Requests that the vector capacity be at least enough to
     * contain n elements.
     * If n is greater than the current vector capacity, the function
     * causes the container to reallocate its storage increasing its
     * capacity to n (or greater).
     */
    void reserve(size_t n);
    /*
     * The request is non-binding, and the container implementation is
     * free to optimize otherwise and leave the vector with a capacity
     * greater than its size.
     */
    void shrink_to_fit();
    T& operator[](size_t n);
    const T& operator[](size_t n) const;

    /*
     * The function automatically checks whether n is within the bounds
     * of valid elements in the vector, throwing an out_of_range
     * exception if it is not (i.e., if n is greater than, or equal to,
     * its size). This is in contrast with member operator[], that does
     * not check against bounds.
     */
    T& at(size_t n);
    const T& at(size_t n) const;
    /*
     * Returns a reference to the first element in the vector.
     * Calling this function on an empty container causes undefined behavior.
     */
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    T* data() noexcept;
    const T* data() const noexcept;
    void assign(size_t n, const T& val);
    // void assign (initializer_list<value_type> il);

    void push_back(const T& val);
    void push_back(T&& val);
    void pop_back();

    // iterator insert (const_iterator position, const value_type& val);
    // iterator insert (const_iterator position, size_type n, const value_type&
    // val); template <class InputIterator> iterator insert (const_iterator
    // position, InputIterator first, InputIterator last); iterator insert
    // (const_iterator position, value_type&& val); iterator insert
    // (const_iterator position, initializer_list<value_type> il); iterator
    // erase (const_iterator position); iterator erase (const_iterator first,
    // const_iterator last);
    void swap(vector& x);
    void clear() noexcept;
    // template <class... Args>
    // iterator emplace (const_iterator position, Args&&... args);
    // template <class... Args>
    //   void emplace_back (Args&&... args);

   private:
    struct vector_data {
        T* begin = nullptr;
        size_t used = 0;
        size_t storage = 0;
        vector_data() {}
        ~vector_data() {
            for (size_t i = 0; i < used; ++i) {
                begin[i].~T();
            }
            if (begin) free(begin);
        }
    };
    vector_data mvector_data;
};

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
    mvector_data.begin = (T*)malloc(n * sizeof(T));
    for (size_t i = 0; i < n; ++i) {
        new (mvector_data.begin + i) T();
    }
    mvector_data.used = n;
    mvector_data.storage = n;
}

template <typename T>
vector<T>::vector(size_t n, const T& val) {
    mvector_data.begin = (T*)malloc(n * sizeof(T));
    for (size_t i = 0; i < n; ++i) {
        new (mvector_data.begin + i) T(val);
    }
    mvector_data.used = n;
    mvector_data.storage = n;
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
    mvector_data.begin = (T*)malloc(rhs.mvector_data.storage * sizeof(T));
    for (size_t index = 0; index < mvector_data.used; ++index) {
        auto* ele = new (mvector_data.begin + index) T(rhs.mvector_data[index]);
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
    for (size_t index = mvector_data.used; index < n; ++index) {
        T* ele = new (mvector_data.begin + index) T();
    }
    for (size_t index = n; index < mvector_data.used; ++index) {
        T* ele = &mvector_data.begin[index];
        mvector_data.begin[index].~T();
    }
    mvector_data.used = n;
}

template <typename T>
void vector<T>::resize(size_t n, const T& val) {
    reserve(n);
    for (size_t index = mvector_data.used; index < n; ++index) {
        auto* ele = new (mvector_data.begin + index) T(val);
    }
    for (size_t index = n; index < mvector_data.used; ++index) {
        mvector_data.begin[index].~T();
    }
    mvector_data.used = n;
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
        T* temp = (T*)malloc(new_storage * sizeof(T));
        for (size_t index = 0; index < mvector_data.used; ++index) {
            auto* ele = new (temp + index) T(mvector_data.begin[index]);
        }

        free(mvector_data.begin);
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
void vector<T>::shrink_to_fit() {
    size_t new_storage = calculate_storage(mvector_data.used);
    if (new_storage != mvector_data.storage) {
        T* temp = (T*)malloc(new_storage * sizeof(T));

        for (size_t index = 0; index < mvector_data.used; ++index) {
            auto* ele = new (temp + index) T(mvector_data.begin[index]);
        }

        free(mvector_data.begin);
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
T& vector<T>::operator[](size_t n) {
    return mvector_data.begin[n];
}

template <typename T>
const T& vector<T>::operator[](size_t n) const {
    return mvector_data.begin[n];
}

template <typename T>
T& vector<T>::at(size_t n) {
    if (n < 0 || n >= mvector_data.used) {
        throw std::out_of_range(".at(): Invalid index!");
    }
    return mvector_data.begin[n];
}

template <typename T>
const T& vector<T>::at(size_t n) const {
    if (n < 0 || n >= mvector_data.used) {
        throw std::out_of_range(".at(): Invalid index!");
    }
    return mvector_data.begin[n];
}

template <typename T>
T& vector<T>::front() {
    return mvector_data.begin[0];
}

template <typename T>
const T& vector<T>::front() const {
    return mvector_data.begin[0];
}

template <typename T>
T& vector<T>::back() {
    return mvector_data.begin[mvector_data.used - 1];
}

template <typename T>
const T& vector<T>::back() const {
    return mvector_data.begin[mvector_data.used - 1];
}

template <typename T>
T* vector<T>::data() noexcept {
    return mvector_data.begin;
}

template <typename T>
const T* vector<T>::data() const noexcept {
    return mvector_data.begin;
}

template <typename T>
void vector<T>::assign(size_t n, const T& val) {
    *this = vector_data(n, val);
}

template <typename T>
void vector<T>::push_back(const T& val) {
    if (mvector_data.used == mvector_data.storage) {
        reserve(mvector_data.used + 1);
    }
    auto* ele = new (mvector_data.begin + mvector_data.used) T(val);
    mvector_data.used++;
}

template <typename T>
void vector<T>::push_back(T&& val) {
    if (mvector_data.used == mvector_data.storage) {
        reserve(mvector_data.used + 1);
    }
    auto* ele = new (mvector_data.begin + mvector_data.used) T(val);
    mvector_data.used++;
}

template <typename T>
void vector<T>::pop_back() {
    mvector_data.begin[mvector_data.used - 1].~T();
    mvector_data.used--;
}

template <typename T>
void vector<T>::swap(vector& x) {
    std::swap(mvector_data.begin, x.mvector_data.begin);
    std::swap(mvector_data.used, x.mvector_data.used);
    std::swap(mvector_data.storage, x.mvector_data.storage);
}

template <typename T>
void vector<T>::clear() noexcept {
    for (auto index = 0; index < mvector_data.used; ++index) {
        mvector_data.begin[index].~T();
    }
    mvector_data.used = 0;
}

}  // namespace cpp::common::container