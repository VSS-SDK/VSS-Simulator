/*
   The MIT License (MIT)

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

#include "iostream"
#include "Header.h"
#include "Simulator.h"
#include "strategies/ModelStrategy.h"
#include "strategies/Strategy.h"
#include "strategies/StrategyBasic.h"
#include "../utils/includes/boost.h"

using namespace std;
//Exemplo de estratégia
class StrategyTest : public ModelStrategy {
public:

	void runStrategy( vector<RobotStrategy*> robotStrategiesTeam, vector<RobotStrategy*> robotStrategiesAdv, btVector3 ballPos ){
		this->robotStrategiesTeam = robotStrategiesTeam;
		this->robotStrategiesAdv = robotStrategiesAdv;
		this->ballPos = ballPos;

		ModelStrategy::runStrategy();

		for(unsigned int i = 0; i < robotStrategiesTeam.size(); i++) {
			float leftWheel, rigthWheel;
			leftWheel = 0;
			rigthWheel = 0;
			robotStrategiesTeam[i]->updateCommand( leftWheel, rigthWheel );
		}
	}
};

bool argParse( int argc, char** argv, bool *fast_travel, int *qtd_of_goals, bool *develop_mode, string* setup_path );

int main( int argc, char *argv[] ){
	bool fast_travel = false;
	int qtd_of_goals = 10;
	bool develop_mode = false;
    string setup_path = "";

	if(argParse( argc, argv, &fast_travel, &qtd_of_goals, &develop_mode, &setup_path )) {
        cout << setup_path << endl;
		Strategy *stratYellowTeam = new Strategy(); //Original strategy
		Strategy *stratBlueTeam = new Strategy(); //Strategy for tests

		Simulator* simulator = new Simulator();
		simulator->runSimulator( argc, argv, stratBlueTeam, stratYellowTeam, fast_travel, qtd_of_goals, develop_mode );
	}else{
		return -1;
	}

	return 0;
}

bool argParse( int argc, char** argv, bool *fast_travel, int *qtd_of_goals, bool *develop_mode, string* setup_path ){
	namespace bpo = boost::program_options;

	// Declare the supported options.
	bpo::options_description desc( "Allowed options" );
	desc.add_options()
	        ( "help,h", "(Optional) produce help message" )
	        ( "fast,f", "(Optional) specify if the time must go 15x faster." )
	        ( "develop,d", "(Optional) turn on the develop mode. the time doesn't count." )
	        ( "qtd_of_goals,g", bpo::value<std::string>()->default_value( "10" ), "(Optional) specify the qtd of goals to end the game. 10 to 100" )
            ( "setup_path,s", bpo::value<string>(), "(Optional) specify a .csv setup file containing the initial robots position");

	bpo::variables_map vm;
	bpo::store( bpo::parse_command_line( argc, argv, desc ), vm );
	bpo::notify( vm );

	if (vm.count( "help" )) {
		std::cout << desc << std::endl;
		return false;
	}

	if (vm.count( "fast" )) {
		*fast_travel = true;
	}

	if (vm.count( "develop" )) {
		*develop_mode = true;
	}

    if(vm.count( "setup_path" )) {
        *setup_path = vm["setup_path"].as<string>();
    }

	stringstream ss;
	ss << vm["qtd_of_goals"].as<string>();
	ss >> *qtd_of_goals;

	if(*qtd_of_goals < 10) {
		*qtd_of_goals = 10;
	}else
	if(*qtd_of_goals > 100) {
		*qtd_of_goals = 100;
	}

	return true;
}
