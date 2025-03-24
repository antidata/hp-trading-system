# High-Performance Trading System

[![Build and Test](https://github.com/antidata/hp-trading-system/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/antidata/hp-trading-system/actions/workflows/build-and-test.yml)

A modern C++ trading system designed for high-performance market data processing and order execution.

## Features

- Real-time market data processing
- Order book management with price-time priority
- Trading engine with position tracking
- High-performance design using modern C++20
- Thread-safe components
- Comprehensive test suite and performance benchmarks

## Requirements

- C++20 compliant compiler (GCC 10+, Clang 12+, or MSVC 2019+)
- CMake 3.15 or higher
- Google Test (for unit tests)
- Google Benchmark (for performance tests)

## Building the Project

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build . --config Release
```

## Running Tests

The project includes both unit tests and performance benchmarks.

### Unit Tests

Unit tests are built using Google Test framework. To run the tests:

```bash
# From the build directory
./tests/trading_tests

# Or using ctest
ctest --output-on-failure
```

### Performance Benchmarks

Performance benchmarks are implemented using Google Benchmark framework. The benchmarks measure:

1. Order Addition Performance
   - Tests adding orders to the order book
   - Measures throughput with different book depths (8 to 8192 orders)
   - Current performance: 170k-3M operations/second

2. Order Matching Performance
   - Tests matching incoming orders against existing orders
   - Uses a fixed price to maximize matching scenarios
   - Current performance: 6M+ operations/second

3. Order Cancellation Performance
   - Tests canceling existing orders
   - Measures performance with different book depths
   - Current performance: 38M-47M operations/second

To run the performance benchmarks:

```bash
# From the build directory
./tests/performance/order_book_perf_test

# With specific options
./tests/performance/order_book_perf_test --benchmark_format=console --benchmark_counters_tabular=true

# Common benchmark options:
#   --benchmark_format=<console|json|csv>  # Output format
#   --benchmark_filter=<regex>             # Run specific benchmarks
#   --benchmark_min_time=<min_time>        # Minimum time per benchmark
#   --benchmark_repetitions=<num_reps>     # Number of repetitions
```

Example benchmark output:
```
--------------------------------------------------------------------------------------------
Benchmark                                  Time             CPU   Iterations UserCounters...
--------------------------------------------------------------------------------------------
BM_OrderBookAddOrder/8                  5904 ns         5865 ns       238171 items_per_second=170.511k/s
BM_OrderBookAddOrder/4096                369 ns          366 ns      1896042 items_per_second=2.73421M/s
BM_OrderBookMatchingScenario/512         162 ns          161 ns      4397198 items_per_second=6.19287M/s
BM_OrderBookCancelOrder/8192            26.1 ns         25.8 ns     30421400 items_per_second=38.7878M/s
```

## Project Structure

```
trading-system/
├── include/                 # Header files
│   ├── market_data/        # Market data processing
│   ├── order_book/         # Order book management
│   └── trading_engine/     # Core trading engine
├── src/                    # Source files
│   ├── market_data/        # Market data implementation
│   ├── order_book/        # Order book implementation
│   ├── trading_engine/    # Trading engine implementation
│   └── main.cpp           # Entry point
├── tests/                  # Test files
│   ├── market_data/       # Market data tests
│   ├── order_book/        # Order book tests
│   ├── trading_engine/    # Trading engine tests
│   └── performance/       # Performance benchmarks
├── CMakeLists.txt         # Main CMake configuration
└── README.md              # This file
```

## Performance Optimization Details

The system uses several optimization techniques:

1. Memory Management
   - PIMPL idiom for better compilation times
   - Smart pointers for safe memory management
   - Pre-allocated vectors for order storage

2. Data Structures
   - `std::map` for price-level organization (ordered access)
   - `std::unordered_map` for order lookup by ID (O(1) access)
   - `std::vector` for orders at each price level

3. Algorithmic Optimizations
   - Price-time priority matching
   - Early exit in matching logic when no matches possible
   - Efficient order cancellation with direct lookup

## License

This project is licensed under the MIT License - see the LICENSE file for details. 