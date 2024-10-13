#pragma once

#include "Room.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Room {
public:

    enum Cell {

        OBSTACLE = -1,
        EMPTY = -2

    };
    
    static const char COMMENT_C = '-';
    static const char VACUUM_C = 'V';
    static const char MESS_C = 'M';
    static const char OBSTACLE_C = '#';
    static const char EMPTY_C = ' ';

    static constexpr pair<int, int> NO_POSITION = {-1, -1};


    Room( const string & fileName );

    void display() const;
    void displayPath( const vector<pair<int, int>> & path );

    const vector<vector<int>> & getRoom() const;
    const pair<int, int> & getVacuumPosition() const;
    const vector<pair<int, int>> & getMessPositions() const;
    const vector<pair<int, int>> & getImportantPoints() const;

    pair<int, int> getShape() const;

private:

    vector<vector<int>> room;
    
    pair<int, int> vacuumPosition = NO_POSITION;
    vector<pair<int, int>> messPositions;
    
    vector<pair<int, int>> importantPoints;

};
