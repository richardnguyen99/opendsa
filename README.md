# OpenDSA

A lightweight, generic collection of data structures and algoritms written in C++. The project is inspired from [the C++ STL](https://en.cppreference.com/w/cpp/container) but with a simplified approach.

> This repository serves solely for the purpose of learning how the C++ STL is implemented. For pracitcal uses, please refer to [the C++ STL](https://en.cppreference.com/w/cpp/container) for efficiency and safety.

## Containers

`opendsa` supports many data structures (or _containers_) that are inspired from the C++ STL as well as other useful data structures such as trie and AVL tree.

### Sequence container

1. Vector ([_doc_](https://en.cppreference.com/w/cpp/container/vector)): a contiguous array that can grows and shrinks

2. Deque ([doc](https://en.cppreference.com/w/cpp/container/deque)): a _doubly-ended queue_

## Usage

1. Your own driver `main.cpp`:

If you want to explore the project by yourself with your own driver:

```cpp
#include <iostream>

#include "vector" // Make sure it's double quotes ", not <>

int main(int argc, const char** argv)
{
    opendsa::vector<int> v = {1,2,3,4,5,6,7,8};

    // other public methods
}
```

Then simply use the pre-defined make command

```sh
make
./main
```

2. Pre-defined drivers:

If you want to see how those implementations are used, you can check out the existing drivers in the `driver/` folder. You can run them as follow:

```sh
make build
./build/deque   # Run the driver for deque
```

3. Use as global headers:

If you want to use globally in your other C/C++ code, you can install to your local machine.

> This repository is still experimental. There are still hidden bugs so be aware to use the headers in your real application. Plus, it can be only installed in Linux-based systems.

```sh
sudo make install
```

Usage:

```cpp
#include <opendsa/deque>    // From opendsa::deque
#include <deque>            // From std::deque

int main(int argc, const char** argv)
{
    opendsa::deque<int> d = { 1, 2, 3, 4, 5, 6, 7, 8 };

    // Other public methods
}
```

If you want to uninstall the project from the your local machine, simply use the following command:

```sh
sudo make uninstall
```

## License

`opendsa` is [MIT licensed](https://github.com/richardnguyen99/opendsa/blob/main/LICENSE)
