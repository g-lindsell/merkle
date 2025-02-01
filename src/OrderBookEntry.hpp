#pragma once
#include <string>

enum class OrderBookType
{
    bid,
    ask,
    sale,
    unknown
};

class OrderBookEntry
{
public:
    std::string timestamp;
    std::string product;
    OrderBookType orderType;
    double price;
    double amount;

    static OrderBookType stringToOrderBookType(const std::string &s);

    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.timestamp < e2.timestamp;
    }

    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price < e2.price;
    }

    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price > e2.price;
    }

    // Declare the constructor here
    /** Create a new OrderBookEntry with the price
     * set to the sent value
     */
    OrderBookEntry(std::string _timestamp,
                   std::string _product,
                   OrderBookType _orderType,
                   double _price,
                   double _amount);
};
