#ifndef Portfolio_h
#define Portfolio_h

using namespace std;
#include <vector>
#include "Position.h"
#include "MarketData.h"
#include <cmath>

class Portfolio
{
private:
    vector<vector<double>> CholeskyMatrix; // cholesky matrix doesn't make complete sense just yet.
    vector<Position> positions;
    double totalValue;

public:
    void addPosition(const Position &p)
    {
        positions.push_back(p);
    }
    vector<Position> getPositions() const{
        return positions;
    }

    vector<vector<double>> getCholeskyMatrix() const{
        return CholeskyMatrix;
    }

    double getTotalValue(MarketData &Data)
    {
        double totalInvestment = 0.0;
        for (int i = 0; i < positions.size(); i++)
        {
            totalInvestment += positions[i].getQuantity() * Data.getCurrentPrice(positions[i].getTicker());
        }
        return totalInvestment;
    }

    void decomposeCholeskyMatrix(const vector<vector<double>> &matrix)
    {
        
        int n = matrix.size();

        
        CholeskyMatrix = vector<vector<double>>(n, vector<double>(n, 0.0));

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                double sum = 0;
                for (int k = 0; k < j; k++)
                {
                    sum += CholeskyMatrix[i][k] * CholeskyMatrix[j][k];
                }

                if (i == j)
                {
                    CholeskyMatrix[i][j] = sqrt(matrix[i][i] - sum);
                }
                else
                {
                    CholeskyMatrix[i][j] = (1.0 / CholeskyMatrix[j][j]) * (matrix[i][j] - sum);
                }
            }
        }
    }
    void setCholeskyMatrix(const vector<vector<double>> &mat)
    {
        decomposeCholeskyMatrix(mat);
    }
};

#endif