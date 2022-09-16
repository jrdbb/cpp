# Template

## Introduction

Generic programming is a method to implement algorithms and data structures in the most general sensible way.

Generic programming helps us to reduce redundancy and programming effort, while it increases reusability and flexibility

## Template Param

* type parameter whenever we use a concrete type
    * int, char, or even a class. Type parameters are the most common type.
* None­type parameter typically values like `3`. 
    * Excluded are floating­point numbers and strings (C­arrays). Since C++20, they work as well, with minor limitations.
* template­-template parameter is required if we pass a template as a parameter to a template.

## Function templates

* The compiler instantiates the function template automatically
* If the compiler can not determine the arguments itself,
they must be specified explicitly.

## Class templates

* Most rules of function templates also apply to class templates.
* Methods can be implemented inside the class or outside.
* Methods implemented outside the class require the template­head before the method as in the class.
* Each instantiation of a class creates a new type.

### Method template

* A method template can be defined inside or outside a class.
* The copy constructor and destructor can not be templates.

### Inheritance

* When deriving a class template, there is a restriction:
  * In the derived class, methods and attributes of the base class
are not automatically available.
* There are three possible solutions:
  * To qualify the method call by the this pointer.
  * Make the name known by using Base<T>::func. 
  * Call the method of the base class directly.

## Thinking in types

* We usually think in values as they are computed during run­time.
* Types are known at compile­time.

## Variadic templates

### example
```
template<typename ...T>
void F(T&... args)
{
  // get the count of args (how many arg)
  constexpr int count = sizeof...(args);
  // unfold
  F(args...);
}
```

### fold and unfold
* unary
  * right fold: (pack op ...)
  * left fold: (... op pack)
* binary
  * right fold: (pack op ... op init)
  * left fold: (init op ... op pack)

* All op must be the same operation
* Parenthesis are required

```
// sum
(args + ...)
// init sum
(1 + ... + args)
// binary cout
(std::cout << ... << args);
```

## Variable templates

constexpr = template<>::value

```
std::is_same_v<>
std::is_pointer_v<>
```
## SFINAE
* Substitution failure is not an error
* Use case
  * one type can satisify two template funcs.
    * a general T version and a specific type. (see the example below)
      ```
      template <typename T>
      void A(T v) {}

      template <>
      void A(double v) {}
      ```
    * in this case, A(float) can't pass compile. because float can fit into both of the two functions.
    * Of course, you can specialize A(float) as well, and make it work. however, people don't like to repeat.
  * solution:
    ```
    template <typename T>
    typename std::enable_if_t<!std::is_floating_point_v<T>> A(T value) {}

    template <typename T>
    typename std::enable_if_t<std::is_floating_point_v<T>> A(T value) {}
    ```

## Tag dispatch

* An empty class (type) to distinguish the template function.
  ```
  struct floatingPoint{}
  struct nonFloatingPoint{}
  template <typename T>
  void A(T value, floatingPoint) {}

  template <typename T>
  void A(T value, nonFloatingPoint) {}

  template <typename T>
  void A(T value) {
    if constexpr (std::is_floating_point_v(T))
    {
      A(value, floatingPoint{});
    }
    else
    {
      A(value, nonFloatingPoint{});
    }
  }
  ```

## Concept
* Can replace most SFINAE
  ```
  template <typename T>
  requires(!std::is_floating_point_v<T>) void A(T value) {}

  template <typename T>
  requires(std::is_floating_point_v<T>) void A(T value) {}
  ```

## Tempalte template parameter
```
template<
  template<typename, typename>
  typename Container,
  typename T,
  typename Allocator
>
void func(Container<T, Allocator> c) {}
```