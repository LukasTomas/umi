#include "Bfsator.h"
#include "SurroundingPoints.h"

#include <deque>
#include <queue>
#include <limits>
#include <algorithm>


// PUBLIC METHODS -----------------------------------------------------------------------------------------------------

Bfsator::Bfsator( Room & room ) : room(room), importantPoints(room.getImportantPoints()) {
    completeGraphDistances.resize(importantPoints.size());
    for( int i = 0; i < completeGraphDistances.size(); i++ )
        completeGraphDistances[i].resize(importantPoints.size());
    srand(time(NULL));
}

void Bfsator::getCompleteGraphDistances( bool useShortest ) {
    queue<int> importantPointsQueue;
    for( int i = 0; i < importantPoints.size(); i++ )
        importantPointsQueue.push(i);
    
    vector< vector<vector<pair<int, int>>> > predecessors(importantPoints.size());
    while( !importantPointsQueue.empty() ) {
        int startingPoint = importantPointsQueue.front();
        // cout << startingPoint << endl; 

        bfs(startingPoint, predecessors[startingPoint]);

        importantPointsQueue.pop();
    }

    // for( int i = 0; i < completeGraphDistances.size(); i++ ) {
    //     for( int j = 0; j < completeGraphDistances[i].size(); j++ )
    //         cout << setw(5) << completeGraphDistances[i][j] << " ";  // setw(5) sets a width of 5 for each number
    //     cout << endl;
    // }

    vector<int> importantaPointsOrder;
    if ( useShortest )
        getBestPath(importantaPointsOrder);
    else
        getRandomPath(importantaPointsOrder);


    int totalDistance = getTotalDistance(importantaPointsOrder);
    int previousPoint = importantaPointsOrder[0];
    
    vector<pair<int, int>> path;
    for( int i = 1; i < importantaPointsOrder.size(); i++ ) {
        vector<pair<int, int>> tmpPath;
        createPath(previousPoint, importantaPointsOrder[i], predecessors, tmpPath);
        path.insert(path.end(), tmpPath.begin(), tmpPath.end());

        previousPoint = importantaPointsOrder[i];
    }
    room.displayPath(path);
    
    cout << endl << "Total distance: " << totalDistance << endl;
    cout << "Path order: ";
    for( int i = 0; i < importantaPointsOrder.size(); i++ )
        cout << importantaPointsOrder[i] << " ";
    cout << endl;
}


// PRIVATE METHODS ----------------------------------------------------------------------------------------------------

void Bfsator::bfs( int startingPoint, vector<vector<pair<int, int>>> & predecessors ) {
    pair<int, int> shape = room.getShape();
    SurroundingPoints surroundingPoints(shape);

    vector<vector<bool>> closed(shape.first);
    predecessors.resize(shape.first);
    vector<vector<int>> distances(shape.first);
    for ( int i = 0; i < shape.first; i++ ) {
        closed[i] = vector<bool>(shape.second, false);
        predecessors[i].reserve(shape.second);
        distances[i].resize(shape.second);
    }

    queue<pair<int, int>> que;
    const pair<int, int> & startingPointCoors = importantPoints[startingPoint];
    que.push({startingPointCoors.first, startingPointCoors.second});
    closed[startingPointCoors.first][startingPointCoors.second] = true;
    predecessors[startingPointCoors.first][startingPointCoors.second] = {-1, -1};
    distances[startingPointCoors.first][startingPointCoors.second] = 0;

    vector<pair<int, int>> pointsAround(4);

    int messCellsFound = 0;
    while( messCellsFound < importantPoints.size() && !que.empty() ) {
        pair<int, int> & currentPos = que.front();

        surroundingPoints.getPoints(currentPos.first, currentPos.second);
        for ( int i = 0; i < surroundingPoints.getPointsSize(); i++ ) {
            const pair<int, int> & point = surroundingPoints.points[i];
            if( room.getRoom()[point.first][point.second] == Room::OBSTACLE )
                continue;

            if ( closed[point.first][point.second] != true ) {
                if ( room.getRoom()[point.first][point.second] >= 0 )
                    messCellsFound++;

                que.push(point);
                closed[point.first][point.second] = true;
                predecessors[point.first][point.second] = {currentPos.first, currentPos.second};
                distances[point.first][point.second] = distances[currentPos.first][currentPos.second] + 1;
            }
        }

        que.pop();
    }

    for( int i = startingPoint+1; i < importantPoints.size(); i++ ) {
        const pair<int, int> & coors = importantPoints[i];
        int distance = distances[coors.first][coors.second];
        completeGraphDistances[startingPoint][i] = distance;
        completeGraphDistances[i][startingPoint] = distance;
    }
}

/** 
 * @param start - index of the starting point
 * @param end - index of the ending point
 * @param predecessors - vector of predecessors for each point
 * @param tmpPath - vector of points that will be filled with the path
 */
void Bfsator::createPath( int start, int end, const vector< vector<vector<pair<int, int>>> > & predecessors, vector<pair<int, int>> & tmpPath ) {
    const vector<vector<pair<int, int>>> & preds = predecessors[start];
    
    const pair<int, int> & startCoors = importantPoints[start];
    const pair<int, int> & endCoors = importantPoints[end];

    pair<int, int> currentPoint = endCoors;
    tmpPath.push_back(currentPoint);
    while( currentPoint != startCoors ) {
        currentPoint = preds[currentPoint.first][currentPoint.second];
        tmpPath.push_back(currentPoint);
    }
    
    reverse(tmpPath.begin(), tmpPath.end());
}

void Bfsator::getBestPath( vector<int> & importantPointsOrder ) {
    for( int i = 0; i < importantPoints.size(); i++ )
        importantPointsOrder.emplace_back(i);

    vector<int> bestPermutation;
    int bestDistance = numeric_limits<int>::max();
    do {
        int currentDistance = getTotalDistance(importantPointsOrder);
        if( currentDistance < bestDistance ) {
            bestDistance = currentDistance;
            bestPermutation = importantPointsOrder;
        }
    } while( next_permutation(importantPointsOrder.begin()+1, importantPointsOrder.end()) );

    importantPointsOrder = bestPermutation;
}

void Bfsator::getRandomPath( vector<int> & importantPointsOrder ) {
    for( int i = 0; i < importantPoints.size(); i++ )
        importantPointsOrder.emplace_back(i);

    random_shuffle(importantPointsOrder.begin()+1, importantPointsOrder.end());
}

int Bfsator::getTotalDistance( const vector<int> & points ) const {
    int totalDistance = 0;

    int previousPoint = points[0];
    for( int i = 1; i < points.size(); i++ ) {
        int distance = completeGraphDistances[previousPoint][points[i]];
        totalDistance += distance;
        previousPoint = points[i];
    }

    return totalDistance;
}

