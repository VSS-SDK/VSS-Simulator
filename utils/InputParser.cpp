//
// Created by manoel on 04/04/18.
//

#include "InputParser.hpp"

vector<btVector3> InputParser::positions;
vector<float> InputParser::angulations;
btVector3 InputParser::positionBall;

bool InputParser::parse(string path){

    ifstream file(path);
    if(!file.is_open()) return false;

    positions.resize(6);
    angulations.resize(6);

    float ang;
    int x, y;

    for(int i=0 ; i<6 ; i++){

        file >> x; file >> y; file >> ang;

        positions[i] = btVector3(x, y, 0);
        angulations[i] = ang;
    }

    file >> x; file >> y;
    positionBall = btVector3(x, y, 0);



}
