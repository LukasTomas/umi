#include "SurroundingPoints.h"

#include <vector>

// PUBLIC METHODS -----------------------------------------------------------------------------------------------------

SurroundingPoints::SurroundingPoints( const pair<int, int> & shape ) : shape(shape) {}

void SurroundingPoints::getPoints( int y, int x ) {
    int point_index = 0;

    vector<pair<int, int>> directions =  { {0, -1}, {0, 1}, {1, 0}, {-1, 0} };

    for ( const pair<int, int> & direction : directions ) {
        pair<int, int> point = {y + direction.first, x + direction.second};
        if ( isInside(point) ) {
            points[point_index] = point;
            point_index++;
        }
    }

    points_size = point_index;
}

size_t SurroundingPoints::getPointsSize() const {
    return points_size;
}


// PRIVATE METHODS ----------------------------------------------------------------------------------------------------

bool SurroundingPoints::isInside( const pair<int, int> & point ) {
    return point.first >= 0 && point.second >= 0 && point.first < shape.first && point.second < shape.second;
}