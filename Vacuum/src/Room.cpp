#include "Room.h"

#include <unistd.h>


// PUBLIC METHODS -----------------------------------------------------------------------------------------------------

Room::Room( const string & fileName ) {
    ifstream ifs(fileName);
    int messNumber = 1;
    string line;
    while( getline(ifs, line) ) {
        // skip comment
        if( line[0] == Room::COMMENT_C )
            continue;

        if( line[line.size() - 1] == '\n' )
            line = line.substr(0, line.size()-1);
        
        vector<int> row;
        for( int i = 0; i < line.size(); i += 2 ) {
            switch( line[i] ) {
                case VACUUM_C:
                    if( vacuumPosition != NO_POSITION ) // vacuum position already set
                        throw invalid_argument("Second vacuum found: " + line);

                    vacuumPosition = {room.size(), row.size()};
                    row.emplace_back(0);
                    break;
                case MESS_C:
                    messPositions.push_back({room.size(), row.size()});
                    row.emplace_back(messNumber++);
                    break;
                case OBSTACLE_C:
                    row.emplace_back(OBSTACLE);
                    break;
                case EMPTY_C:
                    row.emplace_back(EMPTY);
                    break;
                default:
                    throw invalid_argument("Found invalid charachter in line: " + line);
                    break;
            }
        }
        room.emplace_back(row);
    }

    importantPoints.resize(messNumber);
    importantPoints[0] = vacuumPosition;
    for( int i = 1; i < messNumber; i++ ) 
        importantPoints[i] = messPositions[i-1];
}

void Room::display() const {
    cout << string(room[0].size() * (3 + 1), '-') << endl;
    for( const vector<int> & row : room ) {
        for ( auto cell : row ) {
            if( cell == OBSTACLE )
                cout << OBSTACLE_C << " | ";
            else if( cell == EMPTY )
                cout << EMPTY_C << " | ";
            else if( cell == int('.') )
                cout << '.' << " | ";
            else if( cell == int(':') )
                cout << ':' << " | ";
            else
                cout << cell << " | "; 
        }
        cout << endl << string(room[0].size() * (3 + 1), '-') << endl;
    }
}

void Room::displayPath( const vector<pair<int, int>> & path ) {
    for( const pair<int, int> & point : path ) {
        if( room[point.first][point.second] != OBSTACLE ) {
            char c = room[point.first][point.second];
            room[point.first][point.second] = int('.');
            display();
            room[point.first][point.second] = c;
        } else
            display();
        
        usleep(0.25 * 1000 * 1000);
    }
}

const vector<vector<int>> & Room::getRoom() const {
    return room;
}

const pair<int, int> & Room::getVacuumPosition() const {
    return vacuumPosition;
}

const vector<pair<int, int>> & Room::getMessPositions() const {
    return messPositions;
}

const vector<pair<int, int>> & Room::getImportantPoints() const {
    return importantPoints;
}

pair<int, int> Room::getShape() const {
    return {room.size(), room[0].size()};
}
