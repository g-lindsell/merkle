#include "OrderBook.hpp"
#include "CSVReader.hpp"
#include <map>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}
/** return vector of all known products in the dataset */
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (const std::pair<std::string, bool> &productStringBoolPair : prodMap)
    {
        products.push_back(productStringBoolPair.first);
    }

    return products;
}
/** return vector of orders according to the sent filteres */
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
        {
            max = e.price;
        }
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
        {
            min = e.price;
        }
    }
    return min;
}

double OrderBook::getVolume(std::vector<OrderBookEntry> &orders)
{
    double volume = 0;
    for (OrderBookEntry &e : orders)
    {
        volume += e.amount;
    }
    return volume;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp;
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = getEarliestTime();
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
    // sales = []
    std::vector<OrderBookEntry> sales;

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);

    // for ask in asks:
    for (OrderBookEntry &ask : asks)
    {
        // for bid in bids:
        for (OrderBookEntry &bid : bids)
        {
            // if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                // sale = new order()
                OrderBookEntry sale{timestamp, product, OrderBookType::sale, ask.price, 0};

                // now work out how much was sold and
                // create new bids and asks covering
                // anything that was not sold
                // if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    // sale.amount == ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    // # can do no more with this ask
                    // # go onto the next ask
                    break;
                }
                // if bid.amount > ask.amount: # ask is completely gone slice the bid
                else if (bid.amount > ask.amount)
                {
                    // sale.amount = ask.amount
                    sale.amount = ask.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // # we adjust the bid in place
                    // # so it can be used to process the next ask
                    // bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    // # ask is completely gone, so go to next ask
                    break;
                }
                // if bid.amount < ask.amount # bid is completely gone, slice the ask
                else if (bid.amount < ask.amount)
                {
                    // sale.amount = bid.amount
                    sale.amount = bid.amount;
                    // sales.append(sale)
                    sales.push_back(sale);
                    // # update the ask
                    // # and allow further bids to process the remaining amount
                    // ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    // bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    // # some ask remains so go to the next bid
                    continue;
                }
            }
        }
    }

    return sales;
}