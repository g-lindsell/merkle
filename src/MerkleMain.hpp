#pragma once
#include <vector>
#include "OrderBookEntry.hpp"
#include "OrderBook.hpp"

class MerkleMain
{
public:
    MerkleMain() = default;
    /** Call this to start the sim */
    void init();

private:
    void loadOrderBook();
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();
    int getUserOption();
    void processUserOption(int userOption);

    std::string currentTime;

    OrderBook orderBook{"20200317.csv"};
};