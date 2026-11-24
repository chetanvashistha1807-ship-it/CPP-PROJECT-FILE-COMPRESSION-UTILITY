# Huffman File Compression Utility (C++)

A command-line lossless file compressor/decompressor built with Huffman coding, a priority queue (min-heap), a binary tree, and binary file I/O.

## Features

- Reads arbitrary files in binary mode (all 256 byte values are supported).
- Counts byte frequencies and builds a Huffman tree.
- Generates prefix-free variable-length codes.
- Packs bits into bytes rather than writing bit strings as text.
- Stores a file signature, mode, original size, and (for Huffman mode) a 256-entry frequency table.
- Rebuilds the Huffman tree during decompression using the stored frequency table.
- Handles empty files and files containing only one distinct byte value.
- Falls back to storing the original bytes when Huffman output plus metadata would be larger.
- Reports input/output sizes and percentage space change.

## Project structure

| File | Responsibility |
|---|---|
| `main.cpp` | Command-line interface |
| `Node.h/.cpp` | Huffman tree node |
| `Compare.h` | Min-heap comparator |
| `HuffmanTree.h/.cpp` | Tree construction, code generation, frequency analysis |
| `HuffmanData.h` | Frequency, code table, root, and original size |
| `encoding.h` | Compressed file format, encoder, decoder, and statistics |

## Build

Requires a C++11-compatible compiler.

```bash
g++ -std=c++11 main.cpp Node.cpp HuffmanTree.cpp -o compressor
```

On Windows, run `compressor.exe` in Command Prompt or PowerShell.

## Usage

Compress a file (default output is `<input-file>.huf`):

```bash
./compressor c input.txt
```

Choose an output name:

```bash
./compressor c input.txt archive.huf
```

Decompress (default output is `<input-file>.out`):

```bash
./compressor d archive.huf
```

Choose a restored filename:

```bash
./compressor d archive.huf restored.txt
```

## File format

All integer fields are written little-endian.

- 4 bytes: signature `HUF1`
- 1 byte: storage mode (`0` = Huffman, `1` = raw fallback)
- 8 bytes: original file size
- Huffman mode only: 256 unsigned 64-bit byte frequencies, followed by the packed Huffman bitstream
- Raw mode only: original file bytes

The original size tells the decoder exactly how many output bytes to produce, so padding bits in the final encoded byte are ignored.

## Verification

After decompressing, compare the restored file with the original. On Windows:

```powershell
cmd /c fc /b input.txt restored.txt
```

On Linux/macOS:

```bash
cmp input.txt restored.txt
```

A successful comparison means the reconstructed bytes match exactly.

## Notes

The Huffman header is intentionally simple and stores a full 256-entry frequency table. For small files, this metadata can exceed the possible bit savings, so the utility automatically uses raw mode instead. This keeps the format straightforward and guarantees the fallback decision includes metadata overhead.

The implementation uses dynamically allocated Huffman tree nodes and releases the tree after each operation.
