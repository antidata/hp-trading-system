enable_testing()

# Find GTest
find_package(GTest REQUIRED)

# Find Google Benchmark
find_package(benchmark REQUIRED)

# Add test executable
add_executable(unit_tests
    market_data/market_data_test.cpp
    order_book/order_book_test.cpp
    trading_engine/trading_engine_test.cpp
)

target_link_libraries(unit_tests
    PRIVATE
        trading_core
        GTest::GTest
        GTest::Main
)

# Add benchmark executable
add_executable(performance_tests
    benchmarks/order_book_benchmark.cpp
)

target_link_libraries(performance_tests
    PRIVATE
        trading_core
        benchmark::benchmark
)

# Register tests
add_test(NAME unit_tests COMMAND unit_tests)