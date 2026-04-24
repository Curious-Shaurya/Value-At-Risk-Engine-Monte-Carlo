#ifndef MarketData_h
#define MarketData_h

using namespace std;
#include <string>
#include <map>

struct StockData
{
    double currentPrice;
    double volatility;
    double drift;
};

class MarketData
{
private:
    map<string, StockData> stocks;

public:
    void addStock(const string &ticker, double price, double vol, double drift)
    {
        stocks[ticker] = {price, vol, drift};
    }
    double getCurrentPrice(const string &ticker) const
    {
        return stocks.at(ticker).currentPrice;
    }
    double getVolatility(const string &ticker) const{
        return stocks.at(ticker).volatility;
    }
    double getDrift(const string &ticker) const{
        return stocks.at(ticker).drift;
    }

};

#endif