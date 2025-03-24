#include "trading_engine/trading_engine.hpp"
#include <unordered_map>
#include <thread>
#include <atomic>

namespace trading {

class TradingEngine::Impl {
public:
    Impl() = default;
    ~Impl() = default;

    MarketDataHandler market_data_handler_;
    std::unordered_map<std::string, OrderBook> order_books_;
    std::unordered_map<std::string, int> positions_;
    std::atomic<bool> running_{false};
    std::thread engine_thread_;
};

TradingEngine::TradingEngine() : impl_(std::make_unique<Impl>()) {}

TradingEngine::~TradingEngine() {
    stop();
}

void TradingEngine::initialize() {
    // Initialize market data handler subscriptions
    impl_->market_data_handler_.subscribe("AAPL", [this](const MarketData& data) {
        // Handle market data updates
        // TODO: Implement market data handling logic
    });
}

void TradingEngine::start() {
    if (impl_->running_) return;
    
    impl_->running_ = true;
    impl_->engine_thread_ = std::thread([this]() {
        while (impl_->running_) {
            // Main trading engine loop
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    });
}

void TradingEngine::stop() {
    impl_->running_ = false;
    if (impl_->engine_thread_.joinable()) {
        impl_->engine_thread_.join();
    }
}

bool TradingEngine::submit_order(const Order& order) {
    auto& order_book = impl_->order_books_[order.symbol];
    order_book.add_order(order);
    return true;
}

int TradingEngine::get_position(const std::string& symbol) const {
    auto it = impl_->positions_.find(symbol);
    return it != impl_->positions_.end() ? it->second : 0;
}

} // namespace trading 