#pragma once

#include <string>
#include <functional>
#include <memory>

namespace trading {

struct MarketData {
    std::string symbol;
    double price;
    int quantity;
    std::string side;  // "BUY" or "SELL"
    std::string order_type;  // "LIMIT" or "MARKET"
};

class MarketDataHandler {
public:
    MarketDataHandler();
    ~MarketDataHandler();

    // Callback type for market data updates
    using MarketDataCallback = std::function<void(const MarketData&)>;

    // Subscribe to market data updates
    void subscribe(const std::string& symbol, MarketDataCallback callback);

    // Process incoming market data
    void process_market_data(const MarketData& data);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace trading 