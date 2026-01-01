> Minimal implementation of a type-safe tagged union (variant).

## Usage

```cpp
#include "variant.hpp"
#include <string>

int main() {
    variant<int, std::string, double> v1;
    
    variant<int, std::string, double> v2(42);
    variant<int, std::string, double> v3(std::string("hello"));
    variant<int, std::string, double> v4(3.14);
    
    int val = v2.get<0>();
    
    size_t idx = v2.get_index();  // returns 0
    
    v1 = 100;
    v1 = std::string("world");
    v1 = 2.718;
    
    return 0;
}
```

## API

### `variant<Types...>`

| Method | Description |
|--------|-------------|
| `variant()` | Default constructs with the first type |
| `variant(Type&& value)` | Constructs with the given value |
| `get<INDEX>()` | Returns reference to value at compile-time index |
| `get_index()` | Returns runtime index of currently held type |
| `operator=(Type&& value)` | Assigns a new value, changing type if necessary |

## Building

```bash
make test

make benchmark

make clean
```

## Benchmarks

Comparison against `std::variant` (GCC, -O3, 10M iterations):

| Test | variant.hpp | std::variant | Ratio |
|------|--------|--------------|-------|
| Construction (int) | 0.29 ns | 0.29 ns | 1.00x |
| Construction (string) | 3.36 ns | 2.55 ns | 0.76x |
| Get (index 0) | 0.12 ns | 0.26 ns | **2.13x** |
| Get (index 4, deep) | 0.13 ns | 0.24 ns | **1.92x** |
| Assignment (same type) | 0.32 ns | 0.35 ns | 1.09x |
| Assignment (type change) | 0.46 ns | 0.59 ns | **1.27x** |
| Index query | 0.12 ns | 0.25 ns | **2.06x** |

*TODO*
- [] optimmize string copy construction
- [] implementation std::visit
- [] implementation of std::holds_alternative
- [] Exception safety guarantees (valueless_by_exception state)
- [] Copy/move constructors 

