#ifndef monteCarloEngineMT_h
#define monteCarloEngineMT_h

#include <vector>
#include "Portfolio.h"
#include "MarketData.h"
#include <random>
#include <iostream>
#include <thread>
#include <cmath>

using namespace std;

class MonteCarloEngineMT
{
public:
    MonteCarloEngineMT() : rd() {}

    vector<double> runSimulations(const Portfolio &portfolio, const MarketData &marketData, int numSimulations, int numDays);

    void simulationWorker(
        int startIndex, int endIndex, vector<double> &results,
        const Portfolio &portfolio,
        const vector<double> &startPrices, const vector<double> &driftsPart,
        const vector<double> &volatilesPart,
        int numDays, unsigned int seed);

private:
    random_device rd;
};

vector<double> MonteCarloEngineMT::runSimulations(const Portfolio &portfolio, const MarketData &marketData, int numSimulations, int numDays)
{
    size_t numStocks = portfolio.getPositions().size();
    auto positions = portfolio.getPositions();
    auto L = portfolio.getCholeskyMatrix();

    if (numStocks == 0 || L.empty())
        return {};

    vector<double> startPrices(numStocks), driftsPart(numStocks), volatilesPart(numStocks);
    double deltaT = 1.0 / 252.0;

    for (int i = 0; i < numStocks; ++i)
    {
        string t = positions[i].getTicker();
        startPrices[i] = marketData.getCurrentPrice(t);
        double drift = marketData.getDrift(t);
        double vol = marketData.getVolatility(t);
        driftsPart[i] = (drift - 0.5 * vol * vol) * deltaT;
        volatilesPart[i] = vol * sqrt(deltaT);
    }

    vector<double> results(numSimulations);
    unsigned int numThreads = thread::hardware_concurrency();
    vector<thread> cores;
    int simsPerThread = numSimulations / numThreads;

    cout << "Launching " << numThreads << " threads..." << endl;
    cout << "Conducting " << numSimulations << "simulations" << endl; 

    for (int i = 0; i < numThreads; i++)
    {
        int start = i * simsPerThread;
        int end = (i == numThreads - 1) ? numSimulations - 1 : (start + simsPerThread - 1);

        // Launching the thread
        cores.push_back(thread(&MonteCarloEngineMT::simulationWorker, this,
                                 start, end, std::ref(results), std::ref(portfolio),
                                 std::ref(startPrices), std::ref(driftsPart), std::ref(volatilesPart),
                                 numDays, rd()));
    }

    for (auto &t : cores)
    {
        t.join();
    }

    return results;
}

void MonteCarloEngineMT::simulationWorker(int startIndex, int endIndex, vector<double> &results, const Portfolio &portfolio, const vector<double> &startPrices, const vector<double> &driftsPart, const vector<double> &volatilesPart, int numDays, unsigned int seed)
{

    mt19937 localGen(seed); // Unique generator for this thread
    normal_distribution<double> dist(0.0, 1.0);
    size_t numStocks = startPrices.size();
    auto L = portfolio.getCholeskyMatrix();
    auto positions = portfolio.getPositions();
    vector<double> runningPrices;
    vector<double> Z(numStocks);

    for (int simCount = startIndex; simCount <= endIndex; simCount++)
    {
        runningPrices = startPrices;
        for (int dayCount = 0; dayCount < numDays; dayCount++)
        {
            // 1. Generate independent shocks
            for (int s = 0; s < numStocks; s++)
            {
                Z[s] = dist(localGen);
            }
            // 2. Correlate them using Cholesky
            for (int i = 0; i < numStocks; i++)
            {
                double correlatedShock = 0.0;
                for (int j = 0; j <= i; j++)
                {
                    correlatedShock += L[i][j] * Z[j];
                }
                // 3. Update price
                runningPrices[i] *= exp(driftsPart[i] + volatilesPart[i] * correlatedShock);
            }
        }

        // Final Portfolio Value for this simulation
        double totalValue = 0.0;
        for (int i = 0; i < numStocks; i++)
        {
            totalValue += runningPrices[i] * positions[i].getQuantity();
        }
        results[simCount] = totalValue;

    }
}

#endif