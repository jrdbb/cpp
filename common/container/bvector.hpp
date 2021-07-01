#pragma once
#include <math.h>

#include <stdexcept>
#include <vector>

#include "vector.hpp"

namespace cpp::common::container {

struct bit_reference {
    char* mbit_type;
    char mbit_mask;
    bit_reference() : mbit_type(0), mbit_mask(0) {}
    bit_reference(char* bit_type, char bit_mask)
        : mbit_type(bit_type), mbit_mask(bit_mask) {}
    bit_reference(const bit_reference&) = default;
    operator bool() const { return !!(*mbit_type & mbit_mask); }
    bit_reference& operator=(bool val) {
        if (val) {
            *mbit_type |= mbit_mask;
        } else {
            *mbit_type &= ~mbit_mask;
        }
        return *this;
    }
    bool operator==(const bit_reference& x) { return bool(*this) == bool(x); }
    bool operator<(const bit_reference& __x) const {
        return !bool(*this) && bool(__x);
    }

    void flip() _GLIBCXX_NOEXCEPT { *mbit_type ^= mbit_mask; }
};

void swap(bit_reference x, bit_reference y) {
    bool tmp = x;
    x = y;
    y = tmp;
}

void swap(bit_reference x, bool& y) {
    bool tmp = x;
    x = y;
    y = tmp;
}

void swap(bool& x, bit_reference y) {
    bool tmp = x;
    x = y;
    y = tmp;
}
template <>
class vector<bool> {
   public:
    typedef bool value_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef bit_reference reference;
    typedef bool const_reference;
    typedef bit_reference* pointer;
    typedef const bool* const_pointer;
    //   typedef _Bit_iterator				iterator;
    //   typedef _Bit_const_iterator			const_iterator;
    // typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    // typedef std::reverse_iterator<iterator> reverse_iterator;
    vector() = default;
    explicit vector(size_t n);
    vector(size_t n, const bool& val);
    vector(const vector<bool>& x);
    vector(vector<bool>&&);
    vector(std::initializer_list<bool> il);

    ~vector();

    vector& operator=(const vector& x);
    vector& operator=(vector&& x);
    vector& operator=(std::initializer_list<bool> il);

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
    void resize(size_t n, bool val = bool());
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
    reference operator[](size_t n);
    const_reference operator[](size_t n) const;

    /*
     * The function automatically checks whether n is within the bounds
     * of valid elements in the vector, throwing an out_of_range
     * exception if it is not (i.e., if n is greater than, or equal to,
     * its size). This is in contrast with member operator[], that does
     * not check against bounds.
     */
    reference at(size_t n);
    const_reference at(size_t n) const;
    /*
     * Returns a reference to the first element in the vector.
     * Calling this function on an empty container causes undefined behavior.
     */
    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;
    void assign(size_t n, const bool& val);
    // void assign (initializer_list<value_type> il);

    void push_back(bool val);
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
    class iterator
        : public std::iterator<std::random_access_iterator_tag, bool> {
        char* mPointer;
        size_t mOffset;
        char mMask;

       public:
        using it_tag = std::iterator<std::random_access_iterator_tag, bool>;

        iterator(char* pointer, size_t offset)
            : mPointer(pointer), mOffset(offset), mMask(1 << mOffset) {}

        iterator& operator=(const iterator& rhs) {
            mPointer = rhs.mPointer;
            mOffset = rhs.mOffset;
            mMask = rhs.mMask;
            return *this;
        }
        iterator& operator++() {
            if (mOffset < 8) {
                ++mOffset;
                mMask <<= 1;
            } else {
                ++mPointer;
                mOffset = 0;
                mMask = 1;
            }
            return *this;
        }
        iterator operator++(int) {
            iterator retval = *this;
            ++(*this);
            return retval;
        }
        iterator& operator--() {
            if (mOffset > 0) {
                --mOffset;
                mMask >>= 1;
            } else {
                --mPointer;
                mOffset = 7;
                mMask = 0x1 << 7;
            }
            return *this;
        }
        iterator operator--(int) {
            iterator retval = *this;
            --(*this);
            return retval;
        }
        bool operator==(const iterator& other) const {
            return mPointer == other.mPointer && mOffset == other.mOffset &&
                   mMask == other.mMask;
        }
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
        bit_reference operator*() { return bit_reference(mPointer, mMask); }
        iterator& operator+=(typename it_tag::difference_type dist) {
            int poschange = floor((double(mOffset) + dist) / 8);
            mPointer = mPointer + poschange;
            mOffset = (mOffset + dist) % 8;
            mMask = 1 << mOffset;
            return *this;
        }
        iterator operator+(typename it_tag::difference_type dist) const {
            iterator tmp = *this;
            return tmp += dist;
        }
        iterator& operator-=(typename it_tag::difference_type dist) {
            int poschange = floor((double(mOffset) - dist) / 8);
            mPointer = mPointer - poschange;
            mOffset = (mOffset - dist) % 8;
            mMask = 1 << mOffset;
            return *this;
        }
        iterator operator-(typename it_tag::difference_type dist) {
            iterator tmp = *this;
            return tmp -= dist;
        }

        bool operator<(const iterator& rhs) const {
            return (mPointer < rhs.mPointer) ||
                   (mPointer == rhs.mPointer && mOffset < rhs.mOffset);
        }
        bool operator>(const iterator& rhs) const {
            return (mPointer > rhs.mPointer) ||
                   (mPointer == rhs.mPointer && mOffset > rhs.mOffset);
        }
        bool operator<=(const iterator& rhs) const {
            return (mPointer < rhs.mPointer) ||
                   (mPointer == rhs.mPointer && mOffset <= rhs.mOffset);
        }
        bool operator>=(const iterator& rhs) const {
            return (mPointer > rhs.mPointer) ||
                   (mPointer == rhs.mPointer && mOffset >= rhs.mOffset);
        }

        bit_reference operator[](
            typename it_tag::difference_type dist) const {
            return *(*this + dist);
        }

        void swap(iterator& rhs) { std::swap(mPointer, rhs.mPointer); }
        friend iterator operator+(typename it_tag::difference_type dist,
                                  const iterator& iter);
    };
    // class const_iterator
    //     : public std::iterator<std::random_access_iterator_tag, const T> {
    //     const T* mPointer;

    //    public:
    //     using it_tag = std::iterator<std::random_access_iterator_tag, const
    //     T>;

    //     explicit const_iterator(T* pointer) : mPointer(pointer) {}
    //     const_iterator& operator=(const const_iterator& rhs) {
    //         mPointer = rhs.mPointer;
    //         return *this;
    //     }
    //     const_iterator& operator++() {
    //         ++mPointer;
    //         return *this;
    //     }
    //     const_iterator operator++(int) {
    //         const_iterator retval = *this;
    //         ++(*this);
    //         return retval;
    //     }
    //     const_iterator& operator--() {
    //         --mPointer;
    //         return *this;
    //     }
    //     const_iterator operator--(int) {
    //         const_iterator retval = *this;
    //         --(*this);
    //         return retval;
    //     }
    //     bool operator==(const const_iterator& other) const {
    //         return mPointer == other.mPointer;
    //     }
    //     bool operator!=(const const_iterator& other) const {
    //         return !(*this == other);
    //     }
    //     typename it_tag::reference operator*() const { return *mPointer; }
    //     typename it_tag::pointer operator->() const { return mPointer; }

    //     const_iterator operator+(typename it_tag::difference_type dist) {
    //         return const_iterator(mPointer + dist);
    //     }
    //     const_iterator operator-(typename it_tag::difference_type dist) {
    //         return const_iterator(mPointer - dist);
    //     }
    //     typename it_tag::difference_type operator-(const const_iterator& rhs)
    //     {
    //         return mPointer - rhs.mPointer;
    //     }
    //     bool operator<(const const_iterator& rhs) const {
    //         return mPointer < rhs.mPointer;
    //     }
    //     bool operator>(const const_iterator& rhs) const {
    //         return mPointer > rhs.mPointer;
    //     }
    //     bool operator<=(const const_iterator& rhs) const {
    //         return mPointer <= rhs.mPointer;
    //     }
    //     bool operator>=(const const_iterator& rhs) const {
    //         return mPointer >= rhs.mPointer;
    //     }
    //     const_iterator& operator+=(typename it_tag::difference_type dist) {
    //         mPointer += dist;
    //         return *this;
    //     }
    //     const_iterator& operator-=(typename it_tag::difference_type dist) {
    //         mPointer -= dist;
    //         return *this;
    //     }
    //     typename it_tag::reference operator[](
    //         typename it_tag::difference_type dist) {
    //         return *(mPointer + dist);
    //     }
    //     void swap(const_iterator& rhs) { std::swap(mPointer, rhs.mPointer); }
    //     friend const_iterator operator+(typename it_tag::difference_type
    //     dist,
    //                                     const const_iterator& iter);
    // };

    // iterator begin() { return iterator(mvector_data.begin); }
    // const_iterator begin() const { return const_iterator(mvector_data.begin);
    // } iterator end() { return iterator(mvector_data.begin +
    // mvector_data.used); } const_iterator end() const {
    //     return const_iterator(mvector_data.begin + mvector_data.used);
    // }

   private:
    struct vector_data {
        char* begin = nullptr;
        size_t used = 0;
        size_t storage = 0;
        vector_data() {}
        ~vector_data() {
            if (begin) free(begin);
        }
        void destroy_memory() { free(begin); }
    };
    vector_data mvector_data;
};

static size_t bool_calculate_storage(size_t size) {
    /*
     * if on x86
     * asm(bsr)
     */
    size_t storage = 8;
    while (storage < size) {
        storage = storage << 1;
    }
    return storage;
}

// template <typename T>
// bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
//     if (lhs.size() != rhs.size()) {
//         return false;
//     }
//     for (size_t i = 0; i < lhs.size(); ++i) {
//         if (lhs[i] != rhs[i]) {
//             return false;
//         }
//     }
//     return true;
// }

// template <typename T>
// bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
//     return !(lhs == rhs);
// }

vector<bool>::vector(size_t n) : vector(n, false) {}

vector<bool>::vector(size_t n, const bool& val) {
    size_t bytenum = ceil(double(n) / 8);
    mvector_data.begin = (char*)malloc(bytenum);
    if (val) {
        memset(mvector_data.begin, 1 << 8, bytenum);
    } else {
        memset(mvector_data.begin, 0, bytenum);
    }
    mvector_data.used = n;
    mvector_data.storage = 8 * bytenum;
}

vector<bool>::vector(const vector<bool>& rhs) { *this = rhs; }

vector<bool>::vector(vector<bool>&& rhs) {
    *this = std::forward<vector<bool>>(rhs);
}

// template <typename T>
// vector<T>::vector(std::initializer_list<T> il) {
//     reserve(il.size());
//     for (auto& ele : il) {
//         push_back(ele);
//     }
// }

vector<bool>::~vector() {}

size_t vector<bool>::size() const noexcept { return mvector_data.used; }

vector<bool>& vector<bool>::operator=(const vector<bool>& rhs) {
    mvector_data.begin = (char*)malloc(rhs.mvector_data.storage / 8);
    memcpy(mvector_data.begin, rhs.mvector_data.begin,
           rhs.mvector_data.storage / 8);
    mvector_data.used = rhs.mvector_data.used;
    mvector_data.storage = rhs.mvector_data.storage;
    return *this;
}

vector<bool>& vector<bool>::operator=(vector<bool>&& rhs) {
    mvector_data = rhs.mvector_data;
    rhs.mvector_data.begin = nullptr;
    rhs.mvector_data.used = 0;
    rhs.mvector_data.storage = 0;
    return *this;
}

// template <typename T>
// vector<T>& vector<T>::operator=(std::initializer_list<T> il) {
//     *this = vector<T>(il);
//     return *this;
// }

void vector<bool>::resize(size_t n, bool val) {
    reserve(n);
    size_t bytepos;
    char mask;

    for (size_t index = mvector_data.used; index < n; ++index) {
        bytepos = index / 8;
        mask = 1 << (index % 8);
        mvector_data.begin[bytepos] &= ~mask;
    }
    mvector_data.used = n;
}

size_t vector<bool>::capacity() const noexcept { return mvector_data.storage; }

bool vector<bool>::empty() const noexcept { return mvector_data.used == 0; }

void vector<bool>::reserve(size_t n) {
    if (mvector_data.storage < n) {
        size_t new_storage = bool_calculate_storage(n);
        char* temp = (char*)malloc(new_storage / 8);
        memcpy(temp, mvector_data.begin, mvector_data.storage / 8);

        mvector_data.destroy_memory();
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

void vector<bool>::shrink_to_fit() {
    size_t new_storage = bool_calculate_storage(mvector_data.used);
    if (new_storage < mvector_data.storage) {
        char* temp = (char*)malloc(new_storage / 8);
        memcpy(temp, mvector_data.begin, mvector_data.storage / 8);

        mvector_data.destroy_memory();
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

// template <typename T>
// T& vector<T>::operator[](size_t n) {
//     return mvector_data.begin[n];
// }

// template <typename T>
// const T& vector<T>::operator[](size_t n) const {
//     return mvector_data.begin[n];
// }

// template <typename T>
// T& vector<T>::at(size_t n) {
//     if (n < 0 || n >= mvector_data.used) {
//         throw std::out_of_range(".at(): Invalid index!");
//     }
//     return mvector_data.begin[n];
// }

// template <typename T>
// const T& vector<T>::at(size_t n) const {
//     if (n < 0 || n >= mvector_data.used) {
//         throw std::out_of_range(".at(): Invalid index!");
//     }
//     return mvector_data.begin[n];
// }

// template <typename T>
// T& vector<T>::front() {
//     return mvector_data.begin[0];
// }

// template <typename T>
// const T& vector<T>::front() const {
//     return const_cast<vector<T>*>(this)->front();
// }

// template <typename T>
// T& vector<T>::back() {
//     return mvector_data.begin[mvector_data.used - 1];
// }

// template <typename T>
// const T& vector<T>::back() const {
//     return const_cast<vector<T>*>(this)->back();
// }

void vector<bool>::assign(size_t n, const bool& val) { *this = vector(n, val); }

void vector<bool>::push_back(bool val) {
    if (mvector_data.used == mvector_data.storage) {
        reserve(mvector_data.used + 1);
    }
    size_t bytepos = mvector_data.used / 8;
    char mask = 1 << mvector_data.used % 8;
    if (val) {
        mvector_data.begin[bytepos] |= mask;
    } else {
        mvector_data.begin[bytepos] &= ~(mask);
    }
    mvector_data.used++;
}

void vector<bool>::pop_back() { mvector_data.used--; }

void vector<bool>::swap(vector<bool>& x) {
    std::swap(mvector_data.begin, x.mvector_data.begin);
    std::swap(mvector_data.used, x.mvector_data.used);
    std::swap(mvector_data.storage, x.mvector_data.storage);
}

void vector<bool>::clear() noexcept { mvector_data.used = 0; }

// template <typename T>
// typename vector<T>::iterator operator+(
//     typename vector<T>::iterator::difference_type dist,
//     const typename vector<T>::iterator& iter) {
//     return iter + dist;
// }

// template <typename T>
// typename vector<T>::const_iterator operator+(
//     typename vector<T>::const_iterator::difference_type dist,
//     const typename vector<T>::const_iterator& iter) {
//     return iter + dist;
// }

}  // namespace cpp::common::container