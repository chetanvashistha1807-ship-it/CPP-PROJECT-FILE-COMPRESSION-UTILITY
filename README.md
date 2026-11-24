# File Compression Utility — C++

A console-based file compression utility built in **C++**, using the **Huffman Coding algorithm** to perform lossless data compression.

This project is primarily a hands-on learning exercise: the goal is to understand and implement important **Data Structures and Algorithms concepts by building a practical application**, rather than studying them only in isolation. Huffman Coding provides a way to bring together greedy algorithms, binary trees, min-heaps, priority queues, recursion, pointers, and binary file handling in one project.

**Author:** Chetan Vashistha

---

## About the Project

The objective is to build a file compression utility from scratch using C++ and Huffman Coding.

Huffman Coding is a greedy, lossless compression algorithm. It assigns variable-length binary codes to byte values according to their frequency: values that occur more frequently receive shorter codes, while less frequent values receive longer codes. The resulting codes are prefix-free, which makes decoding unambiguous.

The project was developed incrementally. Each part was implemented to understand a particular data structure or programming concept, and then integrated into the complete compression and decompression workflow.

## Why This Project?

The main purpose of this project is to apply core DSA concepts in a practical setting.

Instead of implementing a binary tree, priority queue, recursion, or pointer-based structure as a standalone exercise, this project uses them together to solve a real problem. It also demonstrates how these concepts fit into a modular, multi-file C++ application.

The project focuses on understanding:

- How a greedy algorithm makes local choices to construct a Huffman Tree.
- How a priority queue / min-heap helps repeatedly retrieve the two lowest-frequency nodes.
- How a binary tree represents the generated codes.
- How recursion can traverse that tree and generate codes.
- How pointers and dynamic memory are used to create and manage tree nodes.
- How arrays can be used for frequency counting and code lookup.
- How binary file I/O and bit manipulation can store encoded data compactly.
- How the original data can be reconstructed without information loss.

## How Huffman Coding Works

Huffman Coding uses the frequency of each byte to build a binary tree.

1. Count how often each byte occurs in the input.
2. Create a leaf node for every byte with a non-zero frequency.
3. Insert those nodes into a min-heap implemented using a priority queue.
4. Remove the two nodes with the lowest frequencies.
5. Create a parent node whose frequency is the sum of those two nodes, and make them its children.
6. Insert the parent back into the priority queue.
7. Repeat until one node remains. This node is the root of the Huffman Tree.
8. Traverse the tree recursively: moving left adds `0`, moving right adds `1`. The path to each leaf is that byte's Huffman code.

Because no leaf's code is a prefix of another leaf's code, the encoded bitstream can be decoded unambiguously.

## Data Structures and Concepts Used

| Data structure / concept | Where it is used | Why it is used |
|---|---|---|
| **Binary Tree** | Huffman Tree nodes | Represents the relationship between combined frequencies and provides the paths used to generate and decode codes. |
| **Min-Heap / Priority Queue** | Tree construction | Efficiently retrieves the two nodes with the smallest frequencies at every step of the greedy algorithm. |
| **Array (256 entries)** | Byte-frequency table and code table | A byte has 256 possible values (0–255), so an array provides direct indexing for counting frequencies and looking up codes. |
| **Strings** | Huffman code table | Stores the sequence of `0` and `1` values generated for each byte before those bits are packed into the output. |
| **Pointers and dynamic memory** | Huffman Tree nodes | Nodes are created dynamically as the tree is built; child pointers connect the tree, and the tree is explicitly released after use. |
| **Recursion** | Code generation, tree deletion | Traverses the tree to generate codes and recursively frees its dynamically allocated nodes. |
| **Greedy Algorithm** | Huffman Tree construction | Combines the two currently lowest-frequency nodes at each step to construct the Huffman Tree. |

### Other C++ concepts applied

- Structures and constructors
- Functions and modular programming
- Header files and source files
- STL containers and custom comparators
- Binary file streams (`ifstream` / `ofstream`)
- Bit manipulation and bit packing
- Exception handling
- Memory management
- Basic size calculations and validation

## Implementation Decisions

A few implementation choices were made to keep the project understandable while still making the utility functional.

### 1. Count all 256 possible byte values

The input is read in **binary mode**, and the frequency table has 256 entries. This means the program works with byte values from 0 to 255, rather than assuming that the input contains only ordinary text characters. It can therefore process text, images, and other binary files.

### 2. Use a priority queue as a min-heap

Huffman construction repeatedly needs the two nodes with the lowest frequencies. A priority queue with a custom comparator keeps the lowest-frequency node at the top, avoiding the need to scan the entire collection for the minimum on every iteration.

### 3. Store the frequency table in the compressed file

During decompression, the program needs to rebuild the same Huffman Tree. This implementation stores the 256 byte frequencies in the archive and reconstructs the tree from them. This keeps the format and reconstruction logic straightforward, at the cost of a fixed metadata overhead.

### 4. Pack bits into bytes

Huffman codes are sequences of bits. Writing the characters `'0'` and `'1'` directly would store each bit as a full byte and defeat much of the purpose of compression. The encoder therefore packs eight encoded bits into each output byte.

### 5. Store the original file size

The archive records the original number of bytes. The decoder uses this value to know when it has reconstructed the complete file, including when the final packed byte contains unused padding bits.

### 6. Use raw storage when Huffman output would be larger

For small files or files that do not compress well, the Huffman metadata can make the archive larger than the original. The program compares the estimated total sizes, including metadata, and stores the original bytes in raw mode when that is the smaller option. This avoids making an already-small or incompressible file larger than necessary.

### 7. Handle empty and single-byte files

An empty file has no Huffman tree to build. A file containing only one distinct byte produces a tree with a single leaf. Both cases need explicit handling because they do not follow the normal multi-branch tree traversal used for general input.

## Project Structure

The project uses a modular C++ structure, separating declarations, definitions, and responsibilities across files.

```text
CPP-PROJECT-FILE-COMPRESSION-UTILITY/
│
├── main.cpp
├── Node.h
├── Node.cpp
├── Compare.h
├── HuffmanTree.h
├── HuffmanTree.cpp
├── HuffmanData.h
├── encoding.h
├── .gitignore
└── README.md
```

| File | Responsibility |
|---|---|
| `main.cpp` | Command-line entry point; selects compression or decompression based on the arguments. |
| `Node.h` / `Node.cpp` | Declare and define the Huffman Tree node and its constructor. |
| `Compare.h` | Defines the custom comparator used by the priority queue. |
| `HuffmanTree.h` / `HuffmanTree.cpp` | Declare and implement tree construction, recursive code generation, tree cleanup, and frequency analysis. |
| `HuffmanData.h` | Holds the Huffman root, frequency table, code table, and original file size; declares the frequency-analysis function. |
| `encoding.h` | Implements the archive format, bit packing, compression, decompression, and compression statistics. |

## Compression and Decompression Flow

### Compression

1. `main()` reads the command-line arguments and calls `encodeFile()`.
2. `GenerateHuffmanData()` reads the input file in binary mode and counts the frequency of every byte.
3. `BuildHuffmanTree()` builds the tree using the min-heap priority queue.
4. `GenerateCodes()` recursively generates the code for each byte.
5. The encoder calculates the expected Huffman output size, including the header and frequency table.
6. It chooses Huffman mode or raw mode based on which representation is smaller.
7. In Huffman mode, the frequency table and packed bitstream are written. In raw mode, the original bytes are written.
8. The program reports the storage mode, original size, output size, and space change.

### Decompression

1. `main()` calls `decodeFile()` with the archive and output paths.
2. The decoder checks the archive signature and reads its storage mode and original size.
3. For raw mode, it copies the original bytes directly.
4. For Huffman mode, it reads the frequency table and rebuilds the Huffman Tree.
5. It reads the packed bits and traverses the tree to reconstruct the original bytes.
6. It stops after producing the recorded original file size.

## Compressed File Format

The archive uses a simple binary format. Integer fields are stored in little-endian order.

| Field | Size | Description |
|---|---:|---|
| Signature | 4 bytes | Identifies the file as a Huffman utility archive: `HUF1`. |
| Storage mode | 1 byte | `0` = Huffman mode; `1` = raw mode. |
| Original size | 8 bytes | Size of the original file in bytes. |
| Frequency table | 2,048 bytes | Present only in Huffman mode: 256 unsigned 64-bit frequencies. |
| Data | Variable | Packed Huffman bitstream or original bytes, depending on the mode. |

## Build and Run

A C++11-compatible compiler such as GCC / MinGW is required.

### Compile

Run this command from the project directory:

```bash
g++ -std=c++11 main.cpp Node.cpp HuffmanTree.cpp -o compressor
```

On Windows, this creates `compressor.exe`.

### Compress a file

```powershell
.\compressor.exe c input.txt compressed.huf
```

If the output filename is omitted, the program uses `input.txt.huf`.

### Decompress a file

```powershell
.\compressor.exe d compressed.huf restored.txt
```

If the output filename is omitted, the program uses `compressed.huf.out`.

## Testing

After decompressing a file, compare the original and restored files to verify lossless reconstruction.

On Windows PowerShell:

```powershell
cmd /c "fc /b input.txt restored.txt"
```

On Linux / macOS:

```bash
cmp input.txt restored.txt
```

If the comparison reports no differences, the restored file is byte-for-byte identical to the original.

## Learning Outcome

The completed utility brings several DSA topics together in one program: a greedy algorithm builds a binary tree, a min-heap priority queue selects nodes efficiently, recursion generates codes and manages the tree, and pointers connect dynamically allocated nodes. Arrays provide direct byte-frequency and code lookup, while bit manipulation and binary file I/O turn the generated codes into an actual compressed file.

The emphasis of this project is not on building a production-grade archiver or chasing the highest possible compression ratio. It is on understanding how data structures and algorithms work together when implemented in C++ to solve a practical problem.
