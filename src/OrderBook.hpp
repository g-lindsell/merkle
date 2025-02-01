#pragma once

#include "OrderBookEntry.hpp"
#include "CSVReader.hpp"
#include <string>
#include <vector>
#include <algorithm>

class OrderBook
{
public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all known products in the dataset */
    std::vector<std::string> getKnownProducts();
    /** return vector of orders according to the sent filteres */
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                          std::string product,
                                          std::string timestamp);
    /** returns the earliest time in the orderbook */
    std::string getEarliestTime();
    /** returns the next time after the sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     */
    std::string getNextTime(std::string timestamp);
    void insertOrder(OrderBookEntry &order);
    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
    static double getHighPrice(std::vector<OrderBookEntry> &orders);
    static double getLowPrice(std::vector<OrderBookEntry> &orders);
    static double getVolume(std::vector<OrderBookEntry> &orders);

private:
    std::vector<OrderBookEntry> orders;
};