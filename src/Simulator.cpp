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

#include "Simulator.h"

Simulator::Simulator(){
    stratStep = 0;
	loopBullet = 0;
	numRobotsTeam = NUM_ROBOTS_TEAM;

	gameState = new GameState();
    runningPhysics = false;

    for(int i = 0 ; i < 6 ; i++){
        Command cmd(0, 0);
        commands.push_back(cmd);
    }
}

void Simulator::runSimulator(int argc, char *argv[], ModelStrategy *stratBlueTeam, ModelStrategy *stratYellowTeam){
    int numTeams = 0;
	if(stratBlueTeam) {
		this->strategies.push_back(stratBlueTeam);
		numTeams++;
	}
	if(stratYellowTeam){
		this->strategies.push_back(stratYellowTeam);
		numTeams++;
	}

	if(numTeams == 0){
		cout << "You must set a strategy to run the simulator!" << endl;
		exit(1);
	}

	physics = new Physics(numTeams);
	HandleGraphics::initGraphics(physics,strategies,argc,argv);

    vector<RobotPhysics*> gRobots = physics->getAllRobots();

    for(int i = 0; i < physics->getNumTeams();i++){
        vector<RobotStrategy*> robotStrategiesTeam;
        for(int j = 0; j < numRobotsTeam;j++){
            RobotStrategy* robotStrategy = new RobotStrategy(0);
            robotStrategiesTeam.push_back(robotStrategy);
        }
        gameState->robotStrategiesTeam = robotStrategiesTeam;
        gameState->robotStrategiesAdv = robotStrategiesTeam;
    }

    thread_graphics = new thread(bind(&Simulator::runGraphics, this));
    thread_physics = new thread(bind(&Simulator::runPhysics, this));
    thread_strategies = new thread(bind(&Simulator::runStrategies, this));
    thread_send = new thread(bind(&Simulator::runSender, this));
    thread_receive_team1 = new thread(bind(&Simulator::runReceiveTeam1, this));
    thread_receive_team2 = new thread(bind(&Simulator::runReceiveTeam2, this));

    thread_graphics->join();
    thread_physics->join();
    thread_strategies->join();
    thread_send->join();
    thread_receive_team1->join();
    thread_receive_team2->join();
}


void Simulator::runReceiveTeam1(){
    while(true){
        for(int i = 0 ; i < 3 ; i++){
            commands.at(i) = Command(10, 10);
        }
        usleep(33333);
    }
}

void Simulator::runReceiveTeam2(){
    while(true){
        for(int i = 0 ; i < 3 ; i++){
            commands.at(i+3) = Command(10, 10);
        }
        usleep(33333);
    }
}

void Simulator::runSender(){
    Interface interface;
    interface.createSocketSendState(&global_state);
    arbiter.allocateState(&global_state);

    while(1){
        //cout << "teste" << endl;
        global_state = vss_state::Global_State();
        global_state.set_id(0);
        global_state.set_origin(false);

        vss_state::Ball_State *ball_s = global_state.add_balls();
        ball_s->set_x(physics->getBallPosition().getX());
        ball_s->set_y(physics->getBallPosition().getZ());

        vector<RobotPhysics*> listRobots = physics->getAllRobots();
        for(int i = 0 ; i < 3 ; i++){
            vss_state::Robot_State *robot_s = global_state.add_robots_blue();
            btVector3 posRobot = getRobotPosition(listRobots.at(i));
            robot_s->set_x(posRobot.getX());
            robot_s->set_y(posRobot.getZ());
            float rads = atan2(getRobotOrientation(listRobots.at(i)).getZ(),getRobotOrientation(listRobots.at(i)).getX());
            robot_s->set_yaw(rads);
        }

        for(int i = 0 ; i < 3 ; i++){
            vss_state::Robot_State *robot_s = global_state.add_robots_yellow();
            btVector3 posRobot = getRobotPosition(listRobots.at(i+3));
            robot_s->set_x(posRobot.getX());
            robot_s->set_y(posRobot.getZ());
            float rads = atan2(getRobotOrientation(listRobots.at(i+3)).getZ(),getRobotOrientation(listRobots.at(i+3)).getX());
            robot_s->set_yaw(rads);
            //cout << posRobot.getX() << " : " << posRobot.getY() << endl;
        }

        interface.sendState();
        usleep(33333);
    }
}

void Simulator::runGraphics(){
    while(!runningPhysics){
        usleep(1000000.f*timeStep/handTime);
    }
	HandleGraphics::runGraphics();
}

void Simulator::runPhysics(){
    int subStep = 1;
    float standStep = 1.f/60.f;
    int caseWorld = NONE;

    while(!HandleGraphics::getScenario()->getQuitStatus()){
        usleep(1000000.f*timeStep/handTime);

        if(HandleGraphics::getScenario()->getSingleStep() && gameState->sameState ){
            //cout << endl << endl << endl;
            loopBullet++;
            //cout << "--------Ciclo Atual:\t" << loopBullet << "--------" << endl;
            physics->stepSimulation(timeStep,subStep,standStep);
            gameState->sameState = false;
            runningPhysics = true;
            HandleGraphics::getScenario()->setSingleStep(false);

            caseWorld = arbiter.checkWorld();
            switch(caseWorld){
                case GOAL:{
                    //physics->resetRobotPositions();
                    physics->setBallPosition(btVector3(SIZE_WIDTH/2.0, 2.0, SIZE_DEPTH/2.0));
                }break;
                case NONE:{

                }break;
                default:{
                    cerr << "ERROR" << endl;
                }break;
            }
        }
    }
}

void Simulator::runStrategies(){
    btVector3 posTargets[] = {btVector3(SIZE_WIDTH,0,SIZE_DEPTH/2),btVector3(0,0,SIZE_DEPTH/2)};
    int attackDir = 0;
    int framesSec = (int)(1/timeStep);
    for(int i = 0; i < physics->getNumTeams();i++){
        if(posTargets[i].getX() > 0)  attackDir = 1;
        else attackDir = -1;
        strategies[i]->setAttackDir(attackDir);
        strategies[i]->setFramesSec(framesSec);

        for(int j = 0; j < numRobotsTeam;j++){
            int id = i*numRobotsTeam + j;
            physics->getAllRobots()[id]->setTimeStep(timeStep);
        }
            
    }
    while(!HandleGraphics::getScenario()->getQuitStatus()){
        usleep(1000000.f*timeStep/handTime);
        if(HandleGraphics::getScenario()->getSingleStep()){
            if(!gameState->sameState){
                updateWorld();

                if(strategies.size() > 0){
                    btVector3 ballPos = calcRelativePosition(physics->getBallPosition(),strategies[0]->getAttackDir());
                    calcRelativeWorld(gameState->robotStrategiesTeam,strategies[0]->getAttackDir());

                    strategies[0]->runStrategy(gameState->robotStrategiesTeam,gameState->robotStrategiesTeam,ballPos);
                    if(strategies.size() == 2){
                        btVector3 ballPos = calcRelativePosition(physics->getBallPosition(),strategies[1]->getAttackDir());
                        calcRelativeWorld(gameState->robotStrategiesAdv,strategies[1]->getAttackDir());
                        calcRelativeWorld(gameState->robotStrategiesTeam,strategies[0]->getAttackDir());
                        strategies[1]->runStrategy(gameState->robotStrategiesAdv,gameState->robotStrategiesTeam,ballPos);
                    }
                }else{
                    cout << "You must set a strategy to run the simulator!\n" << endl;
                    exit(0);
                }

                for(int i = 0; i < physics->getNumTeams(); i++){
                    for(int j = 0; j < numRobotsTeam; j++){
                        int id = i*numRobotsTeam + j;
                        if(strategies[i]->getAttackDir() == 1){
                            /*float command[2];
                            command[0] = 10;
                            command[1] = 10;*/

                            float command[2] = { commands.at(i).left, commands.at(i).right };

                            physics->getAllRobots()[id]->updateRobot(strategies[i]->getRobotStrategiesTeam()[j]->getCommand());
                            //physics->getAllRobots()[id]->updateRobot(command);
                        }
                        else{
                            //float command[2] = { commands.at(i+3).left, commands.at(i+3).right };
                            float invCommand[2];
                            invCommand[0] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[1];
                            invCommand[1] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[0];
                            //invCommand[0] = 10;
                            //invCommand[1] = 10;

                            //physics->getAllRobots()[id]->updateRobot(command);
                            physics->getAllRobots()[id]->updateRobot(invCommand);
                        } 
                    }
                }

                gameState->sameState = true;
            }
        }
    }
}

void Simulator::updateWorld(){
    vector<RobotPhysics*> gRobots = physics->getAllRobots();

    for(int i = 0; i < physics->getNumTeams();i++){
        vector<RobotStrategy*> robotStrategiesTeam;
        for(int j = 0; j < numRobotsTeam;j++){
            RobotStrategy* robotStrategy;
            int idRobot = i*numRobotsTeam+j;
            robotStrategy = updateLocalPhysics(j, gRobots[idRobot]);
            robotStrategiesTeam.push_back(robotStrategy);
        }
        if(i == 0)  gameState->robotStrategiesTeam = robotStrategiesTeam;
        else    gameState->robotStrategiesAdv = robotStrategiesTeam;
    }
}

RobotStrategy* Simulator::updateLocalPhysics(int id, RobotPhysics* physicRobot){
    RobotStrategy* robotStrategy = new RobotStrategy(id);

    btTransform  transTemp;
    physicRobot->getRigidBody()->getMotionState()->getWorldTransform(transTemp);
    robotStrategy->setPosition(transTemp.getOrigin());

    btVector3 forwardVec = physicRobot->getRaycast()->getForwardVector();

    robotStrategy->setLocalFront(forwardVec);

    return robotStrategy;
}

void Simulator::calcRelativeWorld(vector<RobotStrategy*> robotStrategiesTeam,int attackDir){
        for(int i = 0; i < robotStrategiesTeam.size(); i++){
            robotStrategiesTeam[i]->setPosition(calcRelativePosition(robotStrategiesTeam[i]->getPosition(),attackDir));
            robotStrategiesTeam[i]->setLocalFront(attackDir*robotStrategiesTeam[i]->getLocalFront());
            robotStrategiesTeam[i]->setLocalRight(attackDir*robotStrategiesTeam[i]->getLocalRight());
        }
}

btVector3 Simulator::calcRelativePosition(btVector3 absPos, int attackDir){
    float relX = absPos.getX();
    //cout << attackDir;
    float relZ = absPos.getZ();
    if(attackDir == -1){

        relZ = SIZE_DEPTH - absPos.getZ();
        relX = SIZE_WIDTH - absPos.getX();
    } 
    return btVector3(relX,0,relZ);
}

btVector3 Simulator::getRobotPosition(RobotPhysics* robot){
    btTransform  transTemp;
    robot->getRigidBody()->getMotionState()->getWorldTransform(transTemp);
    return transTemp.getOrigin();
}

btVector3 Simulator::getRobotOrientation(RobotPhysics* robot){
    btVector3 forwardVec = robot->getRaycast()->getForwardVector();

    return forwardVec;
}
