#include "OrderBookEntry.hpp"

/** Define the constructor */
OrderBookEntry::OrderBookEntry(std::string _timestamp,
                               std::string _product,
                               OrderBookType _orderType,
                               double _price,
                               double _amount) : timestamp(_timestamp),
                                                 product(_product),
                                                 orderType(_orderType),
                                                 price(_price),
                                                 amount(_amount) {}

OrderBookType OrderBookEntry::stringToOrderBookType(const std::string &s)
{
    if (s == "ask")
    {
        return OrderBookType::ask;
    }
    if (s == "bid")
    {
        return OrderBookType::bid;
    }
    return OrderBookType::unknown;
}
