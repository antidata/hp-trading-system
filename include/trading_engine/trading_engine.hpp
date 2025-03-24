#pragma once

#include <string>
#include <memory>
#include "market_data/market_data_handler.hpp"
#include "order_book/order_book.hpp"

namespace trading {

class TradingEngine {
public:
    TradingEngine();
    ~TradingEngine();

    // Initialize the trading engine
    void initialize();

    // Start the trading engine
    void start();

    // Stop the trading engine
    void stop();

    // Submit a new order
    bool submit_order(const Order& order);

    // Get the current position for a symbol
    int get_position(const std::string& symbol) const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace trading 