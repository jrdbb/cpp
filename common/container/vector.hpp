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

    void swap(vector& x);

   private:
    struct vector_data {
        T* begin = nullptr;
        size_t used = 0;
        size_t storage = 0;
        vector_data() {}
        ~vector_data() {
            if (begin) delete[] begin;
        }
    };
    vector_data mvector_data;
};
}  // namespace cpp::common::container