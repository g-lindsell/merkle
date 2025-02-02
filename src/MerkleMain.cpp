#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "OrderBookEntry.hpp"
#include "MerkleMain.hpp"
#include "CSVReader.hpp"

void MerkleMain::printMenu()
{
    std::cout << "1: Print help " << std::endl;
    std::cout << "2: Print exchange stats" << std::endl;
    std::cout << "3: Make an offer " << std::endl;
    std::cout << "4: Make a bid " << std::endl;
    std::cout << "5: Print wallet " << std::endl;
    std::cout << "6: Continue " << std::endl;
    std::cout << "============== " << std::endl;
    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkleMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkleMain::printMarketStats()
{
    for (std::string const p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;

        // Get the ask orders
        std::vector<OrderBookEntry> askEntries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        std::cout << "Asks seen: " << askEntries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(askEntries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(askEntries) << std::endl;

        // Get the bid orders
        std::vector<OrderBookEntry> bidEntries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        std::cout << "Bids seen: " << bidEntries.size() << std::endl;
        std::cout << "Max bid: " << OrderBook::getHighPrice(bidEntries) << std::endl;
        std::cout << "Min bid: " << OrderBook::getLowPrice(bidEntries) << std::endl;
        std::cout << "Trading Volume: " << OrderBook::getVolume(bidEntries) << std::endl;
    }
}

void MerkleMain::enterAsk()
{
    std::cout << "Make an Ask - enter the amount: product,price,amount, e.g. ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    // Tokenize the input string
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 3) // Fix this condition based on how the input is tokenized
    {
        std::cout << "Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            // Pass the tokenized input as a vector to stringsToOBE
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens);
            // You can further use obe here, like adding it to the order book
            orderBook.insertOrder(obe);
        }
        catch (const std::exception &e)
        {
            // This will catch any exception thrown by stringsToOBE
            std::cout << "MerkleMain::enterAsk Bad input: " << e.what() << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}

void MerkleMain::enterBid()
{
    std::cout << "Make a Bid - enter the amount: product,price,amount, e.g. ETH/BTC,200,0.5 " << std::endl;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    // Tokenize the input string
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');

    if (tokens.size() != 3) // Fix this condition based on how the input is tokenized
    {
        std::cout << "Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            // Pass the tokenized input as a vector to stringsToOBE
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens);
            // You can further use obe here, like adding it to the order book
            orderBook.insertOrder(obe);
        }
        catch (const std::exception &e)
        {
            // This will catch any exception thrown by stringsToOBE
            std::cout << "MerkleMain::enterBid Bad input: " << e.what() << std::endl;
        }
    }

    std::cout << "You typed: " << input << std::endl;
}

void MerkleMain::printWallet()
{
    std::cout << "Your wallet is empty. " << std::endl;
}

void MerkleMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids("ETH/BTC", currentTime);
    std::cout << "Sales: " << sales.size() << std::endl;
    for (OrderBookEntry &sale : sales)
    {
        std::cout << "Sale price " << sale.price << " amount " << sale.amount << std::endl;
    }
    currentTime = orderBook.getNextTime(currentTime);
}

int MerkleMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::cout << "Type in 1-6" << std::endl;
    std::getline(std::cin, line);
    try
    {
        userOption = std::stoi(line);
    }
    catch (const std::exception &e)
    {
        //
    }
    std::cout << "You selected " << userOption << std::endl;
    return userOption;
}

void MerkleMain::processUserOption(int userOption)
{
    switch (userOption)
    {
    case 1:
        printHelp();
        break;
    case 2:
        printMarketStats();
        break;
    case 3:
        enterAsk();
        break;
    case 4:
        enterBid();
        break;
    case 5:
        printWallet();
        break;
    case 6:
        gotoNextTimeframe();
        break;
    default:
        std::cout << "You did not enter a number between 1 and 6, inclusive, please try again." << std::endl;
    }
}

/** your main function should call this */
void MerkleMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();
    while (true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
}
