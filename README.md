# Huffman Coding Compression Tool

A file compression tool using Huffman coding implemented in both **C** and **Python**. Compresses and decompresses text files with a typical 40-50% compression ratio.

## Features

- **Efficient Compression**: Huffman algorithm with O(n log n) complexity
- **Dual Implementation**: C (CLI) and Python (Web UI) versions
- **Complete**: Encoding, decoding, file I/O
- **Educational**: Great for learning data structures and algorithms

## Quick Start

### C Version
```bash
# Compile
gcc -o huffman src/huffman.c src/priority_queue.c src/main.c

# Run
./huffman
```

### Python Version
```bash
# Install dependencies
pip install -r requirements.txt

# Web interface
python app.py
# Open http://localhost:5000 in your browser
```

## How It Works

1. **Read File** → Count character frequencies
2. **Build Tree** → Huffman algorithm creates optimal tree
3. **Generate Codes** → Assign binary codes to characters
4. **Compress** → Replace characters with binary codes
5. **Decompress** → Reverse the process using the code table

**Example:** Input `"aaabbbcc"` (8 bytes) → Output ~3 bytes = 62.5% compression

## Complexity

- **Time**: O(n log k) for building tree, O(n) for encoding/decoding
- **Space**: O(k) where k = unique characters
- **Typical Compression**: 40-50% for English text

## License

MIT License
