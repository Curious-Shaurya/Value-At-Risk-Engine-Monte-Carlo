#include "Position.h"
#include "Portfolio.h"
#include "MarketData.h"
#include "monteCarloEngine.h"
#include "monteCarloEngineMultiThreading.h"
#include "RiskAnalyser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
void loadPortfolio(const string &filename, Portfolio &portfolio, MarketData &marketData);
vector<vector<double>> loadCorrelationMatrix(const string &filename);

int main()
{
    Portfolio myPortfolio;
    MarketData Data;
    string filename;

    loadPortfolio("portfolio.txt", myPortfolio, Data);
    vector<vector<double>> matrix = loadCorrelationMatrix("correlations.txt");

    cout << "I have detected: " << myPortfolio.getPositions().size() << " stocks in the portfolio" << endl;
    cout << "The initial investment is: " << myPortfolio.getTotalValue(Data) << endl; 

    myPortfolio.setCholeskyMatrix(matrix);

    MonteCarloEngineMT engine;
    int simulations = 100000;
    int days = 252;
    double confidence = 0.99;
    vector<double> results(simulations);
    results = engine.runSimulations(myPortfolio, Data, simulations, days);

    RiskAnalyser Analyser;
    double VaR = Analyser.calculateVaR(results, myPortfolio.getTotalValue(Data), confidence);

    cout << "The 99% VaR for this portfolio is: " << VaR << endl;

    return 0;
}

void loadPortfolio(const string &filename, Portfolio &portfolio, MarketData &marketData)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    string line;

    getline(file, line);
    int lineNumber = 0;
    while (getline(file, line))
    {
        lineNumber++;
        if (line.empty())
            continue;
        stringstream ss(line);
        string ticker, qtyStr, priceStr, driftStr, volStr;

        if (!(getline(ss, ticker, ',') &&
              getline(ss, qtyStr, ',') &&
              getline(ss, priceStr, ',') &&
              getline(ss, driftStr, ',') &&
              getline(ss, volStr, ',')))
        {
            cerr << "Skipping line " << lineNumber << ": Missing columns!" << endl;
            continue;
        }

        portfolio.addPosition(Position(ticker, stod(qtyStr)));
        marketData.addStock(ticker, stod(priceStr), stod(volStr), stod(driftStr));
    }
    file.close();
}

vector<vector<double>> loadCorrelationMatrix(const string &filename)
{
    vector<vector<double>> matrix;
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        vector<double> row;
        stringstream ss(line);
        string value;

        while (getline(ss, value, ','))
        {
            row.push_back(stod(value));
        }
        matrix.push_back(row);
    }
    file.close();
    return matrix;
}