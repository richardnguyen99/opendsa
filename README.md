# Open DSA

Open-source repository for data structure and algorithm implmentations using C/C++.

## Running locally

This repository is supposed to be a place where I host my own implementations of data structures and algorithms. Hence, it only contains only the headers and the sources and leaves the running work to you completely.

1.  Prerequisites

    I'm using `g++` with **c++2017** for everything. If you want to use it, make sure you have the compiler installed correctly.

    My approach for this repository is based on the `C++ STL libraries` to conveniently work with them.

2.  Create a `main.cpp` to run, test and debug manually in local. For example:

    ```cpp
    // main.cpp

    #include <iostream>
    #include <vector>
    #include "sorting.h" 

    int main()
    {
        std::vector<int> data;

        data.push_back(5);
        data.push_back(3);
        data.push_back(1);
        data.push_back(2);

        opendsa::quick_sort(data);

        ...
    }
    
    ```

## Disclaimer

This repository is solely for my learning purposes and is what I want to share with everyone who wants to know more about data structures and algorithms. If you're looking for some practical data structures and algorithms for a real application, I recommend using the `C++ STL libraries` since it's well-supported, well-tested and has a great community. _Use this at your own risk!_

# Contribution

Although I have no guarantee nor support for you to use IRL, every suggestion or found bug is appreciated to make the project error-free. Just make a pull request to change what you think need to be changed.

# Features

- Data structures:
  - [X] Static array
  - [ ] Dynamic array
  - [ ] Stack
  - [ ] Queue
  - [ ] Linked list
    - [ ] Doubly-linked list
    - [ ] Singly-linked list
    - [ ] Reverse a linked list
    - [ ] Insertion
    - [ ] Deletion
  - [X] Tree
    - [X] Binary tree
      - [X] Binary Search Tree
      - [ ] AVL tree
      - [ ] Red-black tree
      - [ ] Check balance
      - [X] Traversing
        - [X] Pre-order
        - [X] In-order
        - [X] Post-order
        - [X] Breadth-first search (BFS)
        - [X] Depth-first search (DFS)
      - [ ] Invert a binary tree
     - [ ] Heap
    - [ ] Trie
  - [ ] Set
  - [ ] Map
  - [ ] Graph
  - [ ] Matrix
  - [ ] String

- Algorithms:
  - [X] Sorting
    - [X] Comparison-based
      - [X] Bubble sort
      - [X] Insertion sort
      - [X] Selection sort
      - [X] Quick sort
      - [X] Merge sort
      - [ ] Heap sort
    - [X] Non comparison-based
      - [ ] Radix sort
      - [X] Counting sort
      - [ ] Bucket sort
  - [ ] Search
    - [ ] Linear search
    - [ ] Binary Search
  - [ ] Dynamic programming
  - [ ] Recursion
  - [ ] Greedy algorithms
  - [ ] Famous algorithms

This repository also supports testing, CI/CD, build and install. Documentation will be out in the future.