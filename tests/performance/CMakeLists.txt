add_executable(order_book_perf_test
    order_book_perf_test.cpp
)

target_link_libraries(order_book_perf_test
    PRIVATE
        trading_core
        benchmark::benchmark
        benchmark::benchmark_main
)

add_test(NAME order_book_perf_test COMMAND order_book_perf_test --benchmark_format=console) 