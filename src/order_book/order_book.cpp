#include "order_book/order_book.hpp"
#include <map>
#include <unordered_map>
#include <algorithm>

namespace trading {

class OrderBook::Impl {
public:
    Impl() : symbol_("") {}
    explicit Impl(const std::string& symbol) : symbol_(symbol) {}
    ~Impl() = default;

    std::string symbol_;
    std::map<double, std::vector<Order>, std::greater<double>> bids_;  // Price -> Orders (descending)
    std::map<double, std::vector<Order>> asks_;  // Price -> Orders (ascending)
    std::unordered_map<std::string, Order> orders_by_id_;

    // Helper function to match orders
    std::vector<Trade> match_order(const Order& order) {
        std::vector<Trade> trades;
        int remaining_quantity = order.quantity;

        if (order.side == "BUY") {
            // Match against asks (sells)
            for (auto ask_it = asks_.begin(); ask_it != asks_.end() && remaining_quantity > 0;) {
                if (ask_it->first > order.price) break;  // Price is too high

                auto& orders = ask_it->second;
                for (auto order_it = orders.begin(); order_it != orders.end() && remaining_quantity > 0;) {
                    int match_quantity = std::min(remaining_quantity, order_it->quantity);
                    trades.push_back(Trade{
                        order.id,
                        order_it->id,
                        symbol_,
                        ask_it->first,
                        match_quantity,
                        "BUY"
                    });

                    remaining_quantity -= match_quantity;
                    order_it->quantity -= match_quantity;

                    if (order_it->quantity == 0) {
                        orders_by_id_.erase(order_it->id);
                        order_it = orders.erase(order_it);
                    } else {
                        ++order_it;
                    }
                }

                if (orders.empty()) {
                    ask_it = asks_.erase(ask_it);
                } else {
                    ++ask_it;
                }
            }
        } else {
            // Match against bids (buys)
            for (auto bid_it = bids_.begin(); bid_it != bids_.end() && remaining_quantity > 0;) {
                if (bid_it->first < order.price) break;  // Price is too low

                auto& orders = bid_it->second;
                for (auto order_it = orders.begin(); order_it != orders.end() && remaining_quantity > 0;) {
                    int match_quantity = std::min(remaining_quantity, order_it->quantity);
                    trades.push_back(Trade{
                        order_it->id,
                        order.id,
                        symbol_,
                        bid_it->first,
                        match_quantity,
                        "SELL"
                    });

                    remaining_quantity -= match_quantity;
                    order_it->quantity -= match_quantity;

                    if (order_it->quantity == 0) {
                        orders_by_id_.erase(order_it->id);
                        order_it = orders.erase(order_it);
                    } else {
                        ++order_it;
                    }
                }

                if (orders.empty()) {
                    bid_it = bids_.erase(bid_it);
                } else {
                    ++bid_it;
                }
            }
        }

        return trades;
    }
};

OrderBook::OrderBook() : impl_(std::make_unique<Impl>()) {}

OrderBook::OrderBook(const std::string& symbol) : impl_(std::make_unique<Impl>(symbol)) {}

OrderBook::~OrderBook() = default;

std::vector<Trade> OrderBook::add_order(const Order& order) {
    // First try to match the order
    std::vector<Trade> trades = impl_->match_order(order);

    // If there's remaining quantity, add to the book
    int matched_quantity = 0;
    for (const auto& trade : trades) {
        matched_quantity += trade.quantity;
    }

    int remaining_quantity = order.quantity - matched_quantity;
    if (remaining_quantity > 0) {
        Order remaining_order = order;
        remaining_order.quantity = remaining_quantity;
        impl_->orders_by_id_[order.id] = remaining_order;
        if (order.side == "BUY") {
            impl_->bids_[order.price].push_back(remaining_order);
        } else {
            impl_->asks_[order.price].push_back(remaining_order);
        }
    }

    return trades;
}

bool OrderBook::cancel_order(const std::string& order_id) {
    auto it = impl_->orders_by_id_.find(order_id);
    if (it == impl_->orders_by_id_.end()) {
        return false;
    }

    const Order& order = it->second;
    if (order.side == "BUY") {
        auto bid_it = impl_->bids_.find(order.price);
        if (bid_it != impl_->bids_.end()) {
            auto& orders = bid_it->second;
            orders.erase(std::remove_if(orders.begin(), orders.end(),
                [&order_id](const Order& o) { return o.id == order_id; }), orders.end());
            if (orders.empty()) {
                impl_->bids_.erase(bid_it);
            }
        }
    } else {
        auto ask_it = impl_->asks_.find(order.price);
        if (ask_it != impl_->asks_.end()) {
            auto& orders = ask_it->second;
            orders.erase(std::remove_if(orders.begin(), orders.end(),
                [&order_id](const Order& o) { return o.id == order_id; }), orders.end());
            if (orders.empty()) {
                impl_->asks_.erase(ask_it);
            }
        }
    }

    impl_->orders_by_id_.erase(it);
    return true;
}

double OrderBook::get_best_bid() const {
    if (impl_->bids_.empty()) return 0.0;
    return impl_->bids_.begin()->first;
}

double OrderBook::get_best_ask() const {
    if (impl_->asks_.empty()) return 0.0;
    return impl_->asks_.begin()->first;
}

double OrderBook::get_spread() const {
    double best_bid = get_best_bid();
    double best_ask = get_best_ask();
    if (best_bid == 0.0 || best_ask == 0.0) return 0.0;
    return best_ask - best_bid;
}

size_t OrderBook::get_bid_depth() const {
    return impl_->bids_.size();
}

size_t OrderBook::get_ask_depth() const {
    return impl_->asks_.size();
}

int OrderBook::get_bid_volume(double price) const {
    auto it = impl_->bids_.find(price);
    if (it == impl_->bids_.end()) return 0;
    
    int total_volume = 0;
    for (const auto& order : it->second) {
        total_volume += order.quantity;
    }
    return total_volume;
}

int OrderBook::get_ask_volume(double price) const {
    auto it = impl_->asks_.find(price);
    if (it == impl_->asks_.end()) return 0;
    
    int total_volume = 0;
    for (const auto& order : it->second) {
        total_volume += order.quantity;
    }
    return total_volume;
}

} // namespace trading 