#pragma once

#include <utility>
using namespace std;

class SurroundingPoints {
public:

    pair<int, int> points[4]; 

    SurroundingPoints( const pair<int, int> & shape );

    void getPoints( int y, int x );

    size_t getPointsSize() const;


private:

    size_t points_size = 4;
    const pair<int, int> shape;

    bool isInside( const pair<int, int> & point );


};