#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace trading {

struct Order {
    std::string id;
    std::string symbol;
    double price;
    int quantity;
    std::string side;  // "BUY" or "SELL"
    std::string order_type;  // "LIMIT" or "MARKET"
};

struct Trade {
    std::string buyer_order_id;
    std::string seller_order_id;
    std::string symbol;
    double price;
    int quantity;
    std::string aggressor_side;  // "BUY" or "SELL"
};

class OrderBook {
public:
    OrderBook();
    OrderBook(const std::string& symbol);
    ~OrderBook();

    // Add a new order to the book and attempt to match it
    std::vector<Trade> add_order(const Order& order);

    // Cancel an existing order
    bool cancel_order(const std::string& order_id);

    // Get the best bid and ask prices
    double get_best_bid() const;
    double get_best_ask() const;

    // Get the current spread
    double get_spread() const;

    // Get order book depth
    size_t get_bid_depth() const;
    size_t get_ask_depth() const;

    // Get total volume at a price level
    int get_bid_volume(double price) const;
    int get_ask_volume(double price) const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace trading 