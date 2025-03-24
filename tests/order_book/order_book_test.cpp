#include <gtest/gtest.h>
#include "order_book/order_book.hpp"

TEST(OrderBookTest, AddAndCancelOrder) {
    trading::OrderBook book("AAPL");
    
    trading::Order order{"1", "AAPL", 150.0, 100, "BUY", "LIMIT"};
    book.add_order(order);
    
    EXPECT_EQ(book.get_best_bid(), 150.0);
    
    EXPECT_TRUE(book.cancel_order("1"));
    EXPECT_EQ(book.get_best_bid(), 0.0);
} 