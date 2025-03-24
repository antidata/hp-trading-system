#include <iostream>
#include <thread>
#include <chrono>
#include "trading_engine/trading_engine.hpp"

int main(int argc, char* argv[]) {
    try {
        trading::TradingEngine engine;
        
        // Initialize the trading engine
        engine.initialize();
        
        // Start the trading engine
        engine.start();
        
        // Main event loop
        while (true) {
            // TODO: Implement main event loop
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 