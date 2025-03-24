find_package(GTest REQUIRED)

add_executable(trading_tests
    market_data/market_data_test.cpp
    order_book/order_book_test.cpp
    trading_engine/trading_engine_test.cpp
)

target_link_libraries(trading_tests
    PRIVATE
        trading_core
        GTest::GTest
        GTest::Main
)

add_test(NAME trading_tests COMMAND trading_tests) 