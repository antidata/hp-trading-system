#include <benchmark/benchmark.h>
#include "order_book/order_book.hpp"
#include <random>
#include <string>

using namespace trading;

class OrderBookBenchmark : public benchmark::Fixture {
protected:
    OrderBook order_book{"TEST"};
    std::mt19937 rng;
    std::uniform_real_distribution<double> price_dist;
    std::uniform_int_distribution<int> size_dist;
    std::uniform_int_distribution<int> order_id_dist;

    void SetUp(const benchmark::State& state) override {
        rng.seed(42); // Fixed seed for reproducibility
        price_dist = std::uniform_real_distribution<double>(90.0, 110.0);
        size_dist = std::uniform_int_distribution<int>(1, 100);
        order_id_dist = std::uniform_int_distribution<int>(1, 1000000);
    }
};

BENCHMARK_F(OrderBookBenchmark, AddOrder)(benchmark::State& state) {
    for (auto _ : state) {
        Order order{
            std::to_string(order_id_dist(rng)),
            "TEST",
            price_dist(rng),
            size_dist(rng),
            "BUY",
            "LIMIT"
        };
        order_book.add_order(order);
    }
}

BENCHMARK_F(OrderBookBenchmark, MatchOrders)(benchmark::State& state) {
    // Pre-populate order book with some orders
    for (int i = 0; i < 1000; ++i) {
        Order buy_order{
            std::to_string(order_id_dist(rng)),
            "TEST",
            price_dist(rng),
            size_dist(rng),
            "BUY",
            "LIMIT"
        };
        Order sell_order{
            std::to_string(order_id_dist(rng)),
            "TEST",
            price_dist(rng),
            size_dist(rng),
            "SELL",
            "LIMIT"
        };
        order_book.add_order(buy_order);
        order_book.add_order(sell_order);
    }

    for (auto _ : state) {
        Order order{
            std::to_string(order_id_dist(rng)),
            "TEST",
            100.0, // Fixed price for consistent matching
            size_dist(rng),
            "BUY",
            "LIMIT"
        };
        order_book.add_order(order);
    }
}

BENCHMARK_F(OrderBookBenchmark, CancelOrder)(benchmark::State& state) {
    std::vector<std::string> order_ids;
    // Pre-populate order book and collect order IDs
    for (int i = 0; i < 1000; ++i) {
        std::string order_id = std::to_string(order_id_dist(rng));
        order_ids.push_back(order_id);
        Order order{
            order_id,
            "TEST",
            price_dist(rng),
            size_dist(rng),
            "BUY",
            "LIMIT"
        };
        order_book.add_order(order);
    }

    size_t index = 0;
    for (auto _ : state) {
        order_book.cancel_order(order_ids[index % order_ids.size()]);
        ++index;
    }
}

BENCHMARK_MAIN();
