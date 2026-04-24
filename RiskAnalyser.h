#ifndef riskAnalyser_h
#define riskAnalyser_h

#include <vector>
#include <algorithm>

using namespace std;

class RiskAnalyser
{

public:
    double calculateVaR(vector<double> &results, double initialInvestment, double confidenceLevel)
    {
        size_t totalSims = results.size();
        size_t index = static_cast<size_t>(totalSims * (1 - confidenceLevel));

        nth_element(results.begin(), results.begin() + index, results.end());
        double VaR = initialInvestment - results[index];

        return VaR;
    }
};

#endif
