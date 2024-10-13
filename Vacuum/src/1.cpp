#include "Room.h"
#include "Bfsator.h"

#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <map>
#include <queue>

#include <stdexcept>
#include <random>
#include <ctime>

#include <iomanip>
#include <unistd.h>

using namespace std;


void printHelp( const string & name ) {
    cout << "Usage: " << name << " [OPTION]" << endl;
    cout << "Options:" << endl;
    cout << "\tfind-shortest\t\tFind the shortest path between all mess cells" << endl;
}

int main( int argc, char * argv[]) {
    printHelp(argv[0]);
    cout << "==========================================================" << endl;

    if( argc > 2 ) {
        cout << "Too many arguments" << endl;
        printHelp(argv[0]);
        exit(1);
    }

    bool useShortest = false;
    if( argc == 2 ) {
        if( string(argv[1]) == "find-shortest" )
            useShortest = true;
        else {
            cout << "Invalid argument" << endl;
            printHelp(argv[0]);
            exit(1);
        }
    }

    Room room("data/2.room");
    room.display();

    usleep(1 * 1000 * 1000);

    Bfsator bfsator(room);
    bfsator.getCompleteGraphDistances(useShortest);

    return 0;
}