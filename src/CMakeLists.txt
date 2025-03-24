add_library(trading_core
    market_data/market_data_handler.cpp
    order_book/order_book.cpp
    trading_engine/trading_engine.cpp
)

target_include_directories(trading_core
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Find pthread
find_package(Threads REQUIRED)

# Add main executable
add_executable(trading_system
    main.cpp
)

target_link_libraries(trading_system
    PRIVATE
        trading_core
        Threads::Threads
) 