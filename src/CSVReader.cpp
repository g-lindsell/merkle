// CSVReader.cpp
#include "CSVReader.hpp"
#include <iostream>
#include <fstream>
#include <exception>

CSVReader::CSVReader() {}

std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename)
{
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{csvFilename};
    std::string line;

    if (!csvFile.is_open())
    {
        std::cerr << "Error: Cannot open file: " << csvFilename << std::endl;
        return entries;
    }

    while (std::getline(csvFile, line))
    {
        try
        {
            if (line.empty())
                continue;
            OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
            entries.push_back(obe);
        }
        catch (const std::exception &e)
        {
            std::cerr << "CSVReader::readCSV bad data: " << e.what() << std::endl;
        }
    }

    std::cout << "CSVReader::readCSV read " << entries.size() << " entries" << std::endl;
    return entries;
}

OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens)
{
    if (tokens.size() != 5)
    {
        std::cerr << "CSVReader::stringsToOBE Bad line: Expected 5 tokens, got " << tokens.size() << std::endl;
        throw std::runtime_error("Invalid token count");
    }

    try
    {
        return OrderBookEntry{
            tokens[0],
            tokens[1],
            OrderBookEntry::stringToOrderBookType(tokens[2]),
            std::stod(tokens[3]),
            std::stod(tokens[4])};
    }
    catch (const std::exception &e)
    {
        std::cerr << "CSVReader::stringsToOBE Bad float data: " << e.what() << std::endl;
        throw;
    }
}

std::vector<std::string> CSVReader::tokenise(const std::string &line, char delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = line.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(delimiter, start);
    }

    tokens.push_back(line.substr(start));
    return tokens;
}
