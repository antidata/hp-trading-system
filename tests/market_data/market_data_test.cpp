#include <gtest/gtest.h>
#include "market_data/market_data_handler.hpp"

TEST(MarketDataHandlerTest, SubscribeAndProcess) {
    trading::MarketDataHandler handler;
    bool callback_called = false;

    handler.subscribe("AAPL", [&callback_called](const trading::MarketData& data) {
        callback_called = true;
        EXPECT_EQ(data.symbol, "AAPL");
        EXPECT_EQ(data.price, 150.0);
        EXPECT_EQ(data.quantity, 100);
    });

    trading::MarketData data{"AAPL", 150.0, 100, "BUY", "LIMIT"};
    handler.process_market_data(data);

    EXPECT_TRUE(callback_called);
} 