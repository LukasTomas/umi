#pragma once

#include "Room.h"

#include <vector>
using namespace std;

class Bfsator {
public:

    Bfsator( Room & room );

    void getCompleteGraphDistances( bool useShortest );

private:

    Room & room;
    // All important graph vertices where a path will be found between
    const vector<pair<int, int>> & importantPoints;

    vector<vector<int>> completeGraphDistances;


    void bfs( int startingPoint, vector<vector<pair<int, int>>> & predecessors );

    void createPath( int start, int end, const vector< vector<vector<pair<int, int>>> > & predecessors, vector<pair<int, int>> & tmpPath );

    void getBestPath( vector<int> & importantPointsOrder );
    void getRandomPath( vector<int> & importantPointsOrder );

    int getTotalDistance( const vector<int> & points ) const;

};
