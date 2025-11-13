# Advanced Memory Allocator in C

[![Built with C](https://img.shields.io/badge/Built%20with-C-blue.svg)]()
[![Systems Programming](https://img.shields.io/badge/Systems-Programming-orange.svg)]()
[![Memory Management](https://img.shields.io/badge/Memory-Management-green.svg)]()
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)]()

A high-performance custom memory allocator implemented from scratch in C, featuring advanced allocation strategies, memory corruption detection, and intelligent reallocation mechanisms.

## 🚀 Features

- **Best-fit Allocation Algorithm** - Minimizes memory fragmentation by finding the most appropriately sized block
- **Bidirectional Coalescing** - Automatically merges adjacent free blocks during deallocation
- **Intelligent Reallocation** - Attempts in-place expansion before falling back to copy-and-move
- **Memory Corruption Detection** - Canary-based integrity verification to detect buffer overflows
- **Virtual Heap Management** - Custom memory pool management without relying on system allocator
- **Robust Error Handling** - Comprehensive edge case handling and null pointer checks

## 🛠️ Technical Implementation

### Core Architecture
```c
struct MemoryBlock {
    uint32_t canary;        // Corruption detection
    size_t size;            // Usable space (excluding metadata)
    int free;               // Allocation status
    MemoryBlock* prev;      // Doubly-linked list previous
    MemoryBlock* next;      // Doubly-linked list next
};