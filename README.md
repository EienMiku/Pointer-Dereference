# Pointer && Dereference

![C++20](https://img.shields.io/badge/standard-C%2B%2B20-blue)
![Header-Only](https://img.shields.io/badge/library-header--only-green)

## Description
A header-only library for type-safe, multi-level pointer management and dereferencing in modern C++20. Simplifies complex pointer-level operations with a clean and constexpr-friendly interface.

## Features
- **Multi-Level Pointer Abstraction**: Create and manage pointers of arbitrary depth.
- **Compile-Time Safety**: Type-safe operations with clear constraints.
- **Flexible Dereferencing**: Recursively dereference pointers with customizable depth.
- **Header-Only Design**: Easy to integrate into any project.
- **C++20 Compatibility**: Fully leverages C++20 features like constraints and concepts.

## Getting Started

### Prerequisites
This library requires a compiler that supports the C++20 standard.

### Installation
Download the `pointer.hpp` and include it.

## Usage

### Creating Multi-Level Pointers
```cpp
#include "pointer.hpp"

int a = 42;
ptr::pointer<int, 123> ptr{ a };

std::cout << ******ptr.get<6>() << std::endl; // 42
```

### Dereferencing Pointers
```cpp
#include "pointer.hpp"

int a = 42;
ptr::pointer<int, 123> ptr{ a };
auto ptr_1 = &a;
auto ptr_2 = &ptr_1;
auto ptr_3 = &ptr_2;
auto ptr_4 = &ptr_3;
auto ptr_5 = &ptr_4;

std::cout << ptr::deref<123>(ptr) << std::endl; //42
std::cout << ptr::deref<5>(ptr_5) << std::endl; //42
```

### Compile-Time Pointer Utilities
```cpp
static_assert(std::is_same_v<ptr::ptr_t<int, 3>, int ***>);
static_assert(std::is_same_v<ptr::deref_t<int *******, 5>, int **>);
```

## API Reference

### Classes
- `ptr::pointer<base_type, ptr_lvl>`
  - Represents a type-safe, multi-level pointer.
  - **Template Parameters**:
    - `base_type`: The base type of the pointer.
    - `ptr_lvl`: The depth of the pointer.

### Type Aliases
- `ptr::ptr_t<base_type, ptr_lvl>`: Resolves the pointer type with the specified depth.
- `ptr::deref_t<ptr_type, deref_lvl>`: Resolves the dereferenced type after the specified levels.

### Functions
- `template<size_t deref_lvl, class ptr_type> constexpr auto ptr::deref(const ptr_type &ptr)`
  - Recursively dereferences a pointer to the specified depth.

## Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes. Ensure all changes are tested and documented.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
