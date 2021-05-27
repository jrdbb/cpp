#include "vector.hpp"

namespace cpp::common::container{

static size_t calculate_storage(size_t size){
    int storage = 1;
    while(storage < size){
        storage = storage << 1;
    }
    return storage;
}

template <typename T>
vector<T>::vector(size_t n){
    mvector_data = new T[n];
    mvector_data.storage = n;
}

template <typename T>
vector<T>::vector(size_t n, const T& val){
    int new_storage = calculate_storage(n);
    mvector_data = new T[new_storage];
    for(int i = 0; i < n; ++i){
        mvector_data.begin[i] = val;
    }
    mvector_data.used = n;
    mvector_data.storage = new_storage;
}

template <typename T>
vector<T>::vector(const vector<T>& x)
{
    mvector_data.begin = new T[x.mvector_data.storage];
    memcpy(mvector_data.begin, x.mvector_data.begin, 
        x.mvector_data.used*sizeof(T));
    mvector_data.used = x.mvector_data.used;
    mvector_data.storage = x.mvector_data.storage;
}


template <typename T>
vector<T>::~vector()
{
    delete [] mvector_data.begin;
}

template <typename T>
size_t vector<T>::size() const noexcept{
    return mvector_data.used;
} 

template <typename T>
void vector<T>::resize(size_t n){
    if (n<= mvector_data.storage) {
        mvector_data.used = n;
    } else{
        int new_storage = calculate_storage(n);
        T* temp = new T[new_storage];
        memcpy(temp, mvector_data.begin, mvector_data.used*sizeof(T));
        delete [] mvector_data.begin;

        mvector_data.begin = temp;
        mvector_data.used = n;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
void vector<T>::resize(size_t n,const T& val){
    if(n <= mvector_data.used){
        mvector_data.used = n;
    } else if (n<= mvector_data.storage) {
        for(int index = mvector_data.used; index < n; ++n){
            mvector_data.begin[index] = n;
        }
        mvector_data.used = n;
    } else{
        int new_storage = calculate_storage(n);
        T* temp = new T[new_storage];
        memcpy(temp, mvector_data.begin, mvector_data.used*sizeof(T));
        for(int index = mvector_data.used; index < n; ++n){
            temp[index] = n;
        }

        delete [] mvector_data.begin;
        mvector_data.begin = temp;
        mvector_data.used = n;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
size_t vector<T>::capacity() const noexcept{
    return mvector_data.storage;
}

template <typename T>
bool vector<T>::empty() const noexcept{
    return mvector_data.used == 0;
}

template <typename T>
void vector<T>::reserve(size_t n){
    if(mvector_data.storage < n){
        int new_storage = calculate_storage(n);
        T* temp = new T[new_storage];
        memcpy(temp, mvector_data.begin, mvector_data.used*sizeof(T));

        delete [] mvector_data.begin;
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}

template <typename T>
void vector<T>::shrink_to_fit(){
    int new_storage = calculate_storage(mvector_data.used);
    if(new_storage != mvector_data.storage){
        T* temp = new T[new_storage];

        memcpy(temp, mvector_data.begin, mvector_data.used*sizeof(T));

        delete [] mvector_data.begin;
        mvector_data.begin = temp;
        mvector_data.storage = new_storage;
    }
}
}