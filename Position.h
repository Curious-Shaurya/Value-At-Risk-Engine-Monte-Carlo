#ifndef position_h
#define position_h

using namespace std;
#include <string>

class Position
{
    string ticker;
    double stockQuantity;

public:
    Position(string t, double q) : ticker(t), stockQuantity(q) {}
    string getTicker() const{
        return ticker;
    }
    double getQuantity(){
        return stockQuantity;
    }
};

#endif