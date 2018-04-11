//
// Created by manoel on 04/04/18.
//

#include "StartPositionsHelper.hpp"

vector<btVector3> StartPositionsHelper::positions = {btVector3( 25, 4, SIZE_DEPTH - 55 ),
                                            btVector3( 35, 4, 30 ),
                                            btVector3( 55, 4, 45 ),

                                            btVector3( SIZE_WIDTH - 15, 4, 55 ),
                                            btVector3( SIZE_WIDTH - 25, 4, SIZE_DEPTH - SIZE_DEPTH / 2.5 + 20 ),
                                            btVector3( SIZE_WIDTH - 55, 4, 85 )};

vector<btVector3> StartPositionsHelper::angulations = {btVector3( 0, 90, 0 ),
                                              btVector3( 0, 90, 0 ),
                                              btVector3( 0, 90, 0 ),
                                              btVector3( 0, -140, 0 ),
                                              btVector3( 0, 90, 0 ),
                                              btVector3( 0, 90, 0 )};

btVector3 StartPositionsHelper::positionBall = btVector3( 85, 0, 65 );

bool StartPositionsHelper::useFile = false;

bool StartPositionsHelper::parse(string path){

    /**
     * File should follow the template:
     * x1 y1 th1
     * x2 y2 th2
     * ...
     * xB yB
     *
     * Where, x1 corresponds to robot's 1 x coordinate and y1 to robot 1 y coordinate
     * Should repeat for every robot and finish with ball's coordinates
     */

    ifstream file(path);
    if(!file.is_open()) return false;

    positions.clear(); angulations.clear();

    positions.resize(6);
    angulations.resize(6);

    useFile = true;

    int ang, x, y;

    for(int i=0 ; i<6 ; i++){
        file >> x; file >> y; file >> ang;

        positions[i] = btVector3(x, 4, y);
        angulations[i] = btVector3(0, ang, 0);
    }

    file >> x; file >> y;
    positionBall = btVector3(x, 0, y);

//    for(auto i : positions){
//        cout << i.x() << " " << i.z() << endl;
//    }
//
//    for(auto i : angulations){
//        cout << i.y() << endl;
//    }
//
//    cout << positionBall.x() << " " << positionBall.z() << endl;

    file.close();

    return true;
}
