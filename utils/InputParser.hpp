//
// Created by manoel on 04/04/18.
//

#ifndef VSS_SIMULATOR_INPUTPARSER_HPP
#define VSS_SIMULATOR_INPUTPARSER_HPP

#include "../src/Header.h"

#include <fstream>

class InputParser {

public:

    static bool parse(string path);

    static vector<btVector3> positions;
    static vector<float> angulations;

    static btVector3 positionBall;
};

#endif //VSS_SIMULATOR_INPUTPARSER_HPP
