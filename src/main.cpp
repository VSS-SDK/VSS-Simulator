/*The MIT License (MIT)

Copyright (c) 2016 Lucas Borsatto Simão

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
*/

#include "Header.h"
#include "Simulator.h"
#include "strategies/ModelStrategy.h"
#include "strategies/Strategy.h"
#include "strategies/StrategyBasic.h"
#include "../utils/includes/boost.h"

//Exemplo de estratégia
class StrategyTest : public ModelStrategy{
public:
    void runStrategy(vector<RobotStrategy*> robotStrategiesTeam,vector<RobotStrategy*> robotStrategiesAdv,btVector3 ballPos){
        this->robotStrategiesTeam = robotStrategiesTeam;
        this->robotStrategiesAdv = robotStrategiesAdv;
        this->ballPos = ballPos;

        ModelStrategy::runStrategy();

        for(int i = 0; i < robotStrategiesTeam.size(); i++){
            float leftWheel, rigthWheel;
            leftWheel = 0;
            rigthWheel = 0;
            robotStrategiesTeam[i]->updateCommand(leftWheel,rigthWheel);
        }
    }
};

int main(int argc, char *argv[]){
    string strategy_team1, strategy_team2;  // Strategies in pattern VSS-SampleStrategy
    int time_step;                          // how fast 1x, 10x, 100x "if possible"
    int qtd_matchs;                         // 1, ..., 10

	Strategy *stratYellowTeam = new Strategy(); //Original strategy
	Strategy *stratBlueTeam = new Strategy(); //Strategy for tests

	Simulator* simulator = new Simulator();
	simulator->runSimulator(argc, argv, stratBlueTeam,stratYellowTeam);

	return 0;
}
