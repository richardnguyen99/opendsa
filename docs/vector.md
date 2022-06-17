# `opendsa::vector`

## Abstraction

`std::vector` is a C++ template class that contains contiguous elements and abstracts memory management by handling dynamic allocation for grow and shrink internally. It also allows many operations to work with elements such as random access and iterators

`opendsa::vector` is inspired by `std::vector` by replicating it in a more simple term. One major that has been in `std::vector` but not introduced in `opendsa::vector` is that user-defined allocator. `opendsa::vector` uses an internal allocator to handle memory management.

## Introduction

`opendsa::vector` behaves like `std::vector`, so it supports most core functions that are provided by `std::vector`. Essentially, all elements are stored in a contiguous fashion, which allows random access and pointer offset operations. So they can be passed to any function and expected to be an element in a traditional array.

The storage is handled by `std::allocator` internally. It will be expanded or shrunk when it needs to and the process will be automatic. To keep it simple, `opendsa::vector` will expand the storage by keep doubling the current size until sufficient.

## Implementation

`opendsa::vector` has three data members that act as pointers to store the location of elements.
