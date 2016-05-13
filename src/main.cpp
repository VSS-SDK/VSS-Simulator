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

bool argParse(int argc, char** argv, string *strategy_team1, string *strategy_team2, int *time_step, int *qtd_matchs);

int main(int argc, char *argv[]){
    string strategy_team1, strategy_team2;  // Strategies in pattern VSS-SampleStrategy
    int time_step;                          // how fast 1x, 10x, 100x "if possible"
    int qtd_matchs;                         // 1, ..., 10


    if(argParse(argc, argv, &strategy_team1, &strategy_team2, &time_step, &qtd_matchs)){
        cout << "OK" << endl;
        return 0;
    }else{
        cout << "ERROR: Something happen" << endl;
        return -1;
    }
    /*if (argParse(argc, argv, &runTime, &plannerType, &objectiveType, &outputFile)){
        plan(runTime, plannerType, objectiveType, outputFile);
        return 0;
    }else{
        // Return with error
        return -1;
    }*/


	Strategy *stratYellowTeam = new Strategy(); //Original strategy
	Strategy *stratBlueTeam = new Strategy(); //Strategy for tests

	Simulator* simulator = new Simulator();
	simulator->runSimulator(argc, argv, stratBlueTeam,stratYellowTeam);

	return 0;
}

bool argParse(int argc, char** argv, string *strategy_team1, string *strategy_team2, int *time_step, int *qtd_matchs)
{
    namespace bpo = boost::program_options;

    // Declare the supported options.
    bpo::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "(Optional) produce help message")
        ("team1,t1", bpo::value<std::string>()->default_value("default"), "(Required) Specify the name of executable from team 1.") 
        ("team2,t2", bpo::value<std::string>()->default_value("default"), "(Required) Specify the name of executable from team 2.")
        ("time_step,ts", bpo::value<unsigned int>()->default_value(1), "(Optional) Specify the time step (1x, 10x, 100x). Default value it's 1x.")
        ("qtd_matchs,m", bpo::value<unsigned int>()->default_value(1), "(Optional) Specify the number of matchs between team 1 and team 2 (1, ..., 10). Default value it's 1.");
    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
    bpo::notify(vm);

    if (vm.count("help")){
        std::cout << desc << std::endl;
        return false;
    }

    *time_step = vm["time_step"].as<unsigned int>();
    if(*time_step < 1 || *time_step > 100){
        cout << "Invalid time_step " << *time_step << endl;
        return false;
    }

    *qtd_matchs = vm["qtd_matchs"].as<unsigned int>();
    if(*qtd_matchs < 1 || *qtd_matchs > 10){
        cout << "Invalid qtd_matchs " << *qtd_matchs << endl;
        return false;
    }

    *strategy_team1 = vm["team1"].as<string>();
    *strategy_team2 = vm["team2"].as<string>();

    return true;
}