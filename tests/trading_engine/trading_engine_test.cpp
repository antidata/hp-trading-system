#include <gtest/gtest.h>
#include "trading_engine/trading_engine.hpp"

TEST(TradingEngineTest, InitializeAndSubmitOrder) {
    trading::TradingEngine engine;
    
    engine.initialize();
    
    trading::Order order{"1", "AAPL", 150.0, 100, "BUY", "LIMIT"};
    EXPECT_TRUE(engine.submit_order(order));
    
    EXPECT_EQ(engine.get_position("AAPL"), 0);
} 