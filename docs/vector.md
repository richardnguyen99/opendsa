# `opendsa::vector`

`opendsa::vector` is an oversimplified version of [the C++ STL vector](https://en.cppreference.com/w/cpp/container/vector), a contigous array that can grow and shrink the size dynamically in an efficient way.

## Design

### Template

`opendsa::vector` is a templated class. So users are required to provide the type of the elements. For example:

```cpp
opendsa::vector<int> ints               // elements are integers
opendsa::vector<std::string> strings    // elements are strings
opendsa::vector<other> others           // elements are user-defined types
```

### Data members

They are many ways to design a vector class that supports contiguous arrays and still provides functionalities to work with the elements stored in the container. However, there are two common ways:

```cpp
// 1st
template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
class vector
{
private:
    _Tp* _start;        // Pointer pointing to the first item
    _Tp* _finish;       // Pointer pointing to the one past the last item
    _TP* _end_storage;  // Pointer pointing to the end of the array
    _Alloc _alloc;
};

// 2nd
template<typename _Tp, typename _Alloc = std::allocator<_Tp>>
class vector
{
private:
    _Tp * _data;            // Pointer pointing to the storage
    std::size_t _size;      // Number of elements stored in the storage
    std::size_t _capacity;  // Number of elements that the storage could hold
    _Alloc _alloc;
};
```

Both ways are validate and easy to implement other functionalities since they're interchangeable, i.e. `_finish = _data + _size` and `_end_storage = _data + _capacity`. However, because `std::vector` uses the first approach, it will be the main implementation for `opendsa::vector`.

Additionally, we use `std::allocator`, by default unless defined by users, to handle the memory management.

### Member types

Before jumping into the implementation, member types should be defined first. _Member types_ (or sometimes called _nested type name_) are just some aliases that stand for some types which are related to class. They're useful in extracting data types in a standard way regardless what type the elements are.

In `opendsa::vector`, member types are defined:

```cpp
template <typename _Tp, typename _Alloc = std::allocator<_T>>
class vector
{
public:
    using allocator_type = std::allocator<_Tp>;
    using pointer   = typename std::allocator_traits<allocator>::pointer;
    using const_pointer =
        typename std::allocator_traits<allocator>::const_pointer;
    using reference       = _Tp &;
    using value_type      = _Tp;
    using const_reference = const _Tp &;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    // Revised data members
    allocator_type  _alloc;
    pointer         _start;
    pointer         _finish;
    pointer         _end_storage;
};
```

Now, when member types have been defined, it's easy extract what types are associated with a specific vector object in a uniformed interface. Let's take an example for `std::vector`:

```cpp
std::vector<int> v{ 1, 2, 3, 4, 5 };
std::vector<int>::size_type size = v.size();    // size = 5
```

The list of member types at this point is not finished. To make it less complicated, only member types associated with avaiable data members, member methods and _trivial_ types exist.

## Constructors

Constructors defines how `opendsa::vector` objects are declared and initialized.

### Default constructor

The default constructor allows users to declare vector objects without giving, or initializing, any values to the objects. An example from `std::vector`:

```cpp
std::vector<int> v;                 // v is currently hodling no element
std::cout << v.size() << " ";
std::cout << v.capaity() << "\n";

// output: 0 0
```

Therefore, the default constructor can be defined as:

```cpp
template <typename _Tp, typename _Alloc = std::allocator<_T>>
class vector
{
public:
    // public member types
    vector() : _start(), _finish(), _end_storage(), _alloc() {}

private:
    // private data members and helpers
};
```

Since there is no element to hold, it's very straight-forward to define the default constructor for `opendsa::vector`: initializing every pointer to `nullptr`.

## References

- [https://stackoverflow.com/questions/14921531/what-is-the-difference-between-memory-allocation-through-new-and-allocator](https://stackoverflow.com/questions/14921531/what-is-the-difference-between-memory-allocation-through-new-and-allocator)
- [https://en.cppreference.com/w/cpp/container/vector](https://en.cppreference.com/w/cpp/container/vector)
