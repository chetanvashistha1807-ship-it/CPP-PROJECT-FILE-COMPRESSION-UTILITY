# File Compression Utility — C++

A console-based file compression utility built in **C++**, using the **Huffman Coding algorithm** to achieve lossless data compression.

This project is being developed as a hands-on learning exercise to understand how compression algorithms work internally and how fundamental C++ concepts can be applied to build a practical application.

> **Current Status:** Phase 1 — Huffman Tree Construction and Code Generation completed. Binary encoding, compression, and decompression are under development.

---

## Table of Contents

- [About the Project](#about-the-project)
- [What is Huffman Coding?](#what-is-huffman-coding)
- [How the Algorithm Works](#how-the-algorithm-works)
- [Current Implementation](#current-implementation)
- [Project Structure](#project-structure)
- [Technologies and Concepts](#technologies-and-concepts)
- [How to Compile and Run](#how-to-compile-and-run)
- [Development Roadmap](#development-roadmap)
- [Learning Objectives](#learning-objectives)

---

## About the Project

The goal of this project is to build a file compression utility from scratch using C++ and Huffman Coding.

Huffman Coding is a lossless compression algorithm that assigns variable-length binary codes to characters based on their frequency of occurrence.

Characters that appear more frequently receive shorter codes, while less frequent characters receive longer codes.

This allows data to be represented using fewer bits than its original representation in many cases.

The project is being developed incrementally, with each stage focusing on understanding and implementing a specific part of the compression process.

## What is Huffman Coding?

Huffman Coding is a greedy algorithm used for lossless data compression.

Unlike fixed-length encoding, where every character receives the same number of bits, Huffman Coding uses variable-length codes.

For example, consider the following hypothetical character frequencies:

| Character | Frequency |
|---|---:|
| A | 45 |
| B | 20 |
| C | 15 |
| D | 10 |
| E | 5 |
| F | 5 |

A Huffman Tree is constructed by repeatedly combining the two nodes with the lowest frequencies.

The resulting tree determines the binary code assigned to each character.

### Important properties

- **Lossless compression:** The original data can be reconstructed exactly.
- **Variable-length encoding:** Different characters receive codes of different lengths.
- **Prefix-free codes:** No character's code is a prefix of another character's code.
- **Greedy construction:** At each step, the two lowest-frequency nodes are combined.

## How the Algorithm Works

The complete compression process will consist of the following stages.

### 1. Frequency Analysis

The input file is read in binary mode.

The frequency of each possible byte value is counted using an array of 256 elements.

Each array index represents a possible byte value, from 0 to 255.

### 2. Huffman Tree Construction

A priority queue is used to efficiently retrieve the two nodes with the lowest frequencies.

The algorithm repeatedly:

1. Extracts the two nodes with the lowest frequencies.
2. Creates a new internal node.
3. Assigns the extracted nodes as its left and right children.
4. Sets the new node's frequency to the sum of their frequencies.
5. Inserts the new node back into the priority queue.

This process continues until only one node remains. That node becomes the root of the Huffman Tree.

### 3. Huffman Code Generation

The tree is traversed recursively to generate binary codes.

- Moving to the left child adds `0`.
- Moving to the right child adds `1`.
- Reaching a leaf node gives the complete code for that character.

The generated codes are stored in an array of 256 strings.

### 4. Binary Encoding — Planned

The generated Huffman codes will be used to convert the original file into a compressed binary representation.

Instead of writing each bit as a character such as `'0'` or `'1'`, the bits will be packed into bytes.

### 5. Decompression — Planned

The compressed file will contain the information required to reconstruct the Huffman Tree and decode the original data.

The decoder will traverse the tree according to the bits in the compressed file and reconstruct the original bytes.

---

## Current Implementation

The first development milestone has been completed.

### Completed features

- [x] Read an input file in binary mode.
- [x] Calculate the frequency of all 256 possible byte values.
- [x] Implement a custom `Node` structure for the Huffman Tree.
- [x] Implement a custom comparator for the priority queue.
- [x] Construct a Huffman Tree using a min-heap.
- [x] Handle empty input and single-character input cases.
- [x] Generate Huffman codes through recursive tree traversal.
- [x] Store the generated codes in an array.
- [x] Organize the implementation into multiple header and source files.

### Features under development

- [ ] Convert Huffman codes into packed binary data.
- [ ] Write compressed data to a binary output file.
- [ ] Store the Huffman Tree information or equivalent decoding metadata.
- [ ] Implement file decompression.
- [ ] Verify that decompressed output matches the original file.
- [ ] Display compression statistics.

---

## Project Structure

The project follows a modular C++ structure, separating declarations, definitions, and responsibilities across multiple files.

```text
File Compression Utility/
│
├── main.cpp
│
├── Node.h
├── Node.cpp
│
├── Compare.h
│
├── HuffmanTree.h
├── HuffmanTree.cpp
│
├── HuffmanData.h
│
├── .gitignore
└── README.md
```

### File responsibilities

| File | Responsibility |
|---|---|
| `main.cpp` | Program entry point and testing |
| `Node.h` | Declaration of the Huffman Tree node |
| `Node.cpp` | Node constructor definition |
| `Compare.h` | Custom comparator for priority queue |
| `HuffmanTree.h` | Huffman Tree function declarations |
| `HuffmanTree.cpp` | Tree construction, file frequency analysis, and code generation |
| `HuffmanData.h` | Structure for storing the Huffman Tree root and generated codes |

Additional files may be introduced as the project develops.

---

## Technologies and Concepts

**Language:** C++

**Compiler:** GCC / MinGW

**Development Environment:** Windows

### C++ concepts used

- Structures and classes
- Pointers and dynamic memory allocation
- Recursion
- Functions and modular programming
- Header files and source files
- STL containers
- Priority queues
- Custom comparators
- File handling and binary file streams
- Arrays and strings
- Standard library algorithms

### Data Structures and Algorithms

- Binary Trees
- Min-Heap
- Priority Queue
- Greedy Algorithms
- Recursive Tree Traversal
- Frequency Counting

Additional concepts such as bit manipulation, binary file I/O, memory management, and time and space complexity will be explored during subsequent development stages.

---

## How to Compile and Run

Make sure you have a C++ compiler such as GCC installed.

### 1. Clone the repository

```bash
git clone https://github.com/chetanvashistha1807-ship-it/CPP-PROJECT-FILE-COMPRESSION-UTILITY.git
```

### 2. Navigate to the project directory

```bash
cd CPP-PROJECT-FILE-COMPRESSION-UTILITY
```

### 3. Compile the project

```bash
g++ main.cpp Node.cpp HuffmanTree.cpp -o compressor
```

### 4. Run the executable

On Windows PowerShell:

```powershell
.\compressor.exe
```

**Note:** The current implementation uses an input filename supplied to `GenerateHuffmanData()`. Make sure the input file exists at the specified path before running the program.

---

## Development Roadmap

The project is being developed in the following stages:

| Phase | Objective | Status |
|---|---|---|
| Phase 1 | Huffman fundamentals, tree construction, and code generation | Completed |
| Phase 2 | Bit manipulation and binary encoding | Planned |
| Phase 3 | Writing the compressed binary file | Planned |
| Phase 4 | Decompression and reconstruction | Planned |
| Phase 5 | Validation, error handling, and compression statistics | Planned |

The focus is on understanding each component before integrating it into the complete application.

---

## Learning Objectives

This project is intended to provide practical experience with:

1. Understanding and implementing a real-world greedy algorithm.
2. Working with binary trees and priority queues.
3. Using pointers and dynamic memory in C++.
4. Designing modular, multi-file C++ applications.
5. Understanding bit-level operations and binary file formats.
6. Implementing both encoding and decoding processes.
7. Analysing algorithmic time and space complexity.
8. Testing data integrity through lossless reconstruction.

---

## Final Goal

The final objective is to build a working command-line utility capable of:

- Accepting an input file.
- Compressing it using Huffman Coding.
- Saving the compressed output as a binary file.
- Decompressing the generated file.
- Reconstructing the original data without information loss.
- Displaying the original size, compressed size, and compression ratio.

This project is a practical exploration of Data Structures, Algorithms, and C++ programming through implementation rather than theory alone.

**Author**
Chetan Vashistha