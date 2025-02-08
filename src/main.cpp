#include "CSVReader.hpp"
#include "OrderBookEntry.hpp"
#include "MerkleMain.hpp"
#include "Wallet.hpp"
#include <iostream>

int main()
{
    // MerkleMain app{};
    // app.init();
    Wallet wallet;
    wallet.insertCurrency("BTC", 10);
    std::cout << "Wallet has BTC " << wallet.containsCurrency("BTC", 10) << std::endl;
    std::cout << wallet.toString() << std::endl;
}