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
opendsa::vector<int> v{ 1, 2, 3, 4, 5 };
opendsa::vector<int>::size_type size = v.size();    // size = 5
```

The list of member types at this point is not finished. To make it less complicated, only member types associated with avaiable data members, member methods and _trivial_ types exist.

## Constructors

Constructors defines how `opendsa::vector` objects are declared and initialized.

### Default constructor

The default constructor allows users to declare vector objects without giving, or initializing, any values to the objects.

#### Example

```cpp
opendsa::vector<int> v;                 // v is currently hodling no element
std::cout << v.size() << " ";
std::cout << v.capaity() << "\n";

// output: 0 0
```

#### Implementation

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

#### Explanation

Since the default constructor allows users to declare vector objects without initializing any value, all the three pointers can point to `nullptr` to indicate that there is no value to hold. This implementation later implies the size and the capacity are 0.

### Count-value constructor

The count-value constructor constructs a vector filled with `count` copies of a given `value`.

#### Example

```cpp
opendsa::vector<int> v(8, 3);
std::cout << v.size() << " ";
std::cout << v.capaity() << "\n";

// Output:
// 8 8
// 3 3 3 3 3 3 3 3
```

#### Implementation

```cpp
#include <memory>

// ...
public:
    // other public member functions
    vector(size_type n, const _Tp &value)
    {
        using traits_t = std::allocator_traits<_Alloc>;

        _start  = traits_t::allocate(_alloc, n);
        for (size_type i = 0; i < n; i++)
            traits_t::construct(_alloc, _start + i, value);

        _finish = _start + n;
        _end    = _start + n;
    }
    // other public member functions
```

#### Explanation

First, `traits_t::allocate` will allocate the spaces needed for `n` copies and assign the starting pointer pointing to the first element to `_start`. Then, `traits_t::construct` will create `n` objects of type `_Tp`  with the value `value`. Finally, `_finish` and `_end` will be marked as the end of the storage.

One note is that this constructor will create a multiple copies of `value`.


### Count default-value constructor

The count default-value constructor constructs a vector filled with `count` default-valued elements.

#### Example

```cpp
opendsa::vector<int> v(8);
std::cout << v.size() << " ";
std::cout << v.capaity() << "\n";

for (const int& e : v)
    std::cout << e << " ";
std::cout << "\n";

// Output:
// 8 8
// 0 0 0 0 0 0 0 0
```

#### Implementation

```cpp
#include <memory>

// ...
public:
    // other public member functions
    vector(size_type n)
    {
        using traits_t = std::allocator_traits<_Alloc>;

        _start  = traits_t::allocate(_alloc, n);
        for (size_type i = 0; i < n; i++)
            traits_t::construct(_alloc, _start + i, value_type());

        _finish = _start + n;
        _end    = _start + n;
    }
    // other public member functions
```

#### Explanation

This constructor behaves exactly as the count-value constructor, except that insteadd of passing a value, the constructor will fill the container with default value of type `_Tp`. If `_Tp` is a user-defined class, its default constructor must be provided.

### Range-fill constructor

This constructor constructs a vector object filled with elements based on a specified range of elements.

#### Examples

```cpp
int array[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
opendsa::vector<int> v(std::begin(array), std::end(array));

std::cout << v.size() << " " << v.capacity() << "\n";
for (const int& e : v)
    std::cout << e << " ";
std::cout << "\n";

// Output:
// 8 8
// 1 2 3 4 5 6 7 8
```

#### Implementation

```cpp
#include <memory>

// ...
public:
    // other public member functions
    template <
        typename _InputIter,
        typename = typename std::enable_if<std::is_convertible<
            typename std::iterator_traits<_InputIter>::iterator_category,
            std::input_iterator_tag>::value>::type>
    constexpr vector(_InputIter first, _InputIter last)
    {
        using traits_t = std::allocator_traits<allocator>;

        const difference_type n = std::distance(first, last);
        _start                  = traits_t::allocate(_alloc, n);
        _finish                 = _start;

        for (auto curr = first; curr != last; curr++)
        {
            traits_t::construct(_alloc, std::addressof(*_finish), *curr);
            _finish++;
        }

        _end = _start + n;
    }
    // other public member functions
```

#### Explanation

Quite tricky! The idea of this constructor is to initialize a vector object with a range of elements. This range could be from other vector objects whose type is either `opendsa::vector` or `std::vector`, array, or any type that supports iterators.

Since this constructor is templated with type `_InputIter`, `_InputIter` can be anything. Thus, it's mandatory to separate it from the count-value constructor in case `_InputIter` is passed in the parameters in the count-value constructor. Therefore, `std::enable_if` is used to make it the most appropriate constructor if `_InputIter` satisfies `std::input_iterator_tag` (or [LegacyInputIterator](https://en.cppreference.com/w/cpp/named_req/InputIterator)) to the ambiguity.

Then, the constructor will extract elements from the range `[first, last)` and insert them into the new vector object. `std::addressof()` in this case guarantees the element that is about to be constructed is in the correct address.

### Initializer-list constructor

This constructor allows initializing a vector object with a _C-style_ list.

#### Example

```cpp
opendsa::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8 };

std::cout << v.size() << " " << v.capacity() << "\n";
for (const int& e : v)
    std::cout << e << " ";
std::cout << "\n";

// Output
// 8 8
// 1 2 3 4 5 6 7 8
```

#### Implementation

```cpp
#include <initializer_list>

vector(std::initializer_list<value_type> init)
{
    using traits_t = std::allocator_traits<allocator>;

    const size_type n = init.size();
    _start                  = traits_t::allocate(_alloc, n);
    _finish                 = _start;

    for (auto curr = init.begin(); curr != init.end(); curr++)
    {
        traits_t::construct(_alloc, std::addressof(*_finish), *curr);
        _finish++;
    }

    _end = _start + n;
}
```

#### Explanation

This constructor's definition behaves exactly like the one in the range-based constructor. But instead of passing two iterators to mark the range, the list will be passed directly so inline arrays are accepted, thus there is no need to create one just to initialize a vector object.

Note that, the initializer list constructor is sometimes mistaken with the count-value constructor since both have similar syntax. For example:

```cpp
opendsa::vector<int> v1{3,4}; // A container holding 3 and 4. Size = 2
opendsa::vector<int> v2(3,4); // A container holding three 4's, or {4,4,4}. Size = 3.
```

### Copy constructor

This constructor uses the deep copying technique to copy each elements from the original vector object to the new one without compromising the data in original when there is modification occurring to the new one.

#### Example

```cpp
opendsa::vector<int> v1{ 1, 2, 3, 4, 5, 6, 7, 8 };
opendsa::vector<int> v2(v1);

std::cout << v2.size() << " " << v2.capacity() << "\n";
for (const int& e : v2)
    std::cout << e << " ";
std::cout << "\n";

// Output
// 8 8
// 1 2 3 4 5 6 7 8
```

#### Implementation

```cpp
vector(const vector& other)
{
    using traits_t = std::allocator_traits<allocator>;

    const difference_type n = other._finish - other._start;
    _start                  = traits_t::allocate(_alloc, n);
    _finish                 = _start;

    for (auto curr = other._start; curr != other._finish; curr++)
    {
        traits_t::construct(_alloc, std::addressof(*_finish), *curr);
        _finish++;
    }

    _end = _start + n;
}
```

#### Explanation

This constructor is similar to the range-based and initializer list constructor in a way that it uses two pointers to mark the range of elements and constructs new elements accordingly.

One thing to note is that the other vector must be passed by reference. If there were no copy constructor defined yet, passing by value would cause an infinite loop because the copy constructor would call itself... over and over again... `const` is used to make sure there is no modification occurring to the original.

### Move constructor

This constructor allows transfering ownership of the data held by one vector object to the new one without copying.

#### Example

```cpp
opendsa::vector<int> dummy(8,5);
opendsa::vector<int> v(std::move(dummy));

std::cout << "Dummy: " << dummy.size() << " " << dummy.capacity() << "\n";
std::cout << "Vector: " << v.size() << " " << v.capacity() << "\n";

for (const int& e : dummy)
    std::cout << e << " ";
std::cout << "\n";
for (const int& e : v)
    std::cout << e << " ";
std::cout << "\n";

// Output:
// Dummy: 0 0
// Vector: 8 8
//
// 5 5 5 5 5 5 5 5
```

#### Implementation

```cpp
vector(vector&& other)
{
    this->_start  = other._start;
    this->_finish = other._finish;
    this->_end    = other._end;

    other._start  = pointer();
    other._finish = pointer();
    other._end    = pointer();
}
```

#### Explanation

The main idea of move construct is to transfer ownership without new allocations. Instead, all the pointers from `other` are copied to those in `this`, and assigned with `nullptr`.

### Destructor

The destructor is to clean up and return the allocated memory back to the memory pool when a vector object goes out of scope.

#### Implementation

```cpp
~vector()
{
    using traits_t          = std::allocator_traits<allocator>;
    const difference_type n = std::distance(_start, _finish);

    for (auto curr = _start; curr != _finish; curr++)
        traits_t::destroy(_alloc, std::addressof(*curr));

    _finish = _start;
    traits_t::deallocate(_alloc, _start, n);
}
```

#### Explanation

First, the destructor calls the destructor of type `_Tp` to destroy each individual element in case `_Tp` associates with dynamic allocation and needs to be reclaimed.

Finally, it frees all the allocated storage associated with the object.

## References

- [https://stackoverflow.com/questions/14921531/what-is-the-difference-between-memory-allocation-through-new-and-allocator](https://stackoverflow.com/questions/14921531/what-is-the-difference-between-memory-allocation-through-new-and-allocator)
- [https://en.cppreference.com/w/cpp/container/vector](https://en.cppreference.com/w/cpp/container/vector)
- [https://en.cppreference.com/w/cpp/language/sfinae](https://en.cppreference.com/w/cpp/language/sfinae)
