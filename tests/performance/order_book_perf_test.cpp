#include <gtest/gtest.h>
#include <benchmark/benchmark.h>
#include <random>
#include <chrono>
#include "order_book/order_book.hpp"

using namespace trading;

// Helper function to generate random orders
Order generate_random_order(std::mt19937& gen, const std::string& symbol, int id) {
    std::uniform_real_distribution<> price_dist(90.0, 110.0);
    std::uniform_int_distribution<> qty_dist(1, 1000);
    std::uniform_int_distribution<> side_dist(0, 1);

    return Order{
        std::to_string(id),
        symbol,
        price_dist(gen),
        qty_dist(gen),
        side_dist(gen) ? "BUY" : "SELL",
        "LIMIT"
    };
}

static void BM_OrderBookAddOrder(benchmark::State& state) {
    OrderBook book("AAPL");
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::vector<Order> orders;

    // Pre-generate orders
    for (int i = 0; i < state.range(0); ++i) {
        orders.push_back(generate_random_order(gen, "AAPL", i));
    }

    size_t idx = 0;
    for (auto _ : state) {
        book.add_order(orders[idx % orders.size()]);
        ++idx;
    }

    state.SetItemsProcessed(state.iterations());
}

static void BM_OrderBookMatchingScenario(benchmark::State& state) {
    OrderBook book("AAPL");
    std::mt19937 gen(42);
    std::vector<Order> orders;

    // Pre-generate matching orders
    for (int i = 0; i < state.range(0); ++i) {
        double price = 100.0;  // Fixed price for better matching
        orders.push_back(Order{
            std::to_string(i),
            "AAPL",
            price,
            100,
            i % 2 ? "BUY" : "SELL",
            "LIMIT"
        });
    }

    size_t idx = 0;
    for (auto _ : state) {
        auto trades = book.add_order(orders[idx % orders.size()]);
        benchmark::DoNotOptimize(trades);
        ++idx;
    }

    state.SetItemsProcessed(state.iterations());
}

static void BM_OrderBookCancelOrder(benchmark::State& state) {
    OrderBook book("AAPL");
    std::mt19937 gen(42);
    std::vector<std::string> order_ids;

    // Pre-populate the book
    for (int i = 0; i < state.range(0); ++i) {
        auto order = generate_random_order(gen, "AAPL", i);
        book.add_order(order);
        order_ids.push_back(order.id);
    }

    size_t idx = 0;
    for (auto _ : state) {
        book.cancel_order(order_ids[idx % order_ids.size()]);
        ++idx;
    }

    state.SetItemsProcessed(state.iterations());
}

// Register benchmarks
BENCHMARK(BM_OrderBookAddOrder)->Range(8, 8<<10);
BENCHMARK(BM_OrderBookMatchingScenario)->Range(8, 8<<10);
BENCHMARK(BM_OrderBookCancelOrder)->Range(8, 8<<10);

BENCHMARK_MAIN(); 