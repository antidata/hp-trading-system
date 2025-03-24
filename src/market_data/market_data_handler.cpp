#include "market_data/market_data_handler.hpp"
#include <unordered_map>
#include <vector>

namespace trading {

class MarketDataHandler::Impl {
public:
    Impl() = default;
    ~Impl() = default;

    std::unordered_map<std::string, std::vector<MarketDataCallback>> callbacks_;
};

MarketDataHandler::MarketDataHandler() : impl_(std::make_unique<Impl>()) {}

MarketDataHandler::~MarketDataHandler() = default;

void MarketDataHandler::subscribe(const std::string& symbol, MarketDataCallback callback) {
    impl_->callbacks_[symbol].push_back(std::move(callback));
}

void MarketDataHandler::process_market_data(const MarketData& data) {
    auto it = impl_->callbacks_.find(data.symbol);
    if (it != impl_->callbacks_.end()) {
        for (const auto& callback : it->second) {
            callback(data);
        }
    }
}

} // namespace trading 