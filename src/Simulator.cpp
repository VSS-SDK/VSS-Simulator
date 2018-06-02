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

#include "Simulator.h"
#include "functional"
#include "Interfaces/IControlReceiver.h"
#include "Communications/ControlReceiver.h"
#include "../utils/StartPositionsHelper.hpp"

Simulator::Simulator(){
    loopBullet = 0;
    numRobotsTeam = NUM_ROBOTS_TEAM;

    gameState = new GameState();

    for(int i = 0; i < 6; i++) {
        Command cmd( 0, 0 );
        commands.push_back( cmd );
    }

    status_team_1 = status_team_2 = -1;

    qtd_of_goals = 10;

    finish_match = false;

    goals_team_1 = 0;
    goals_team_2 = 0;

    paused = !StartPositionsHelper::useFile;
    stateSender = new vss::StateSender();
}

void Simulator::runSimulator( int argc, char *argv[], ModelStrategy *stratBlueTeam, ModelStrategy *stratYellowTeam, bool fast_travel, int qtd_of_goals, bool develop_mode ){
    this->qtd_of_goals = qtd_of_goals;
    this->develop_mode = develop_mode;

    if(!fast_travel) {
        timeStep = 1.f / 60.f;
        handTime = 1.f;
    }else{
        timeStep = 1.f / 60.f;
        handTime = 15.f;
    }

    int numTeams = 0;
    if(stratBlueTeam) {
        this->strategies.push_back( stratBlueTeam );
        numTeams++;
    }

    if(stratYellowTeam) {
        this->strategies.push_back( stratYellowTeam );
        numTeams++;
    }

    if(numTeams == 0) {
        cout << "You must set a strategy to run the simulator!" << endl;
        exit( 1 );
    }

    physics = new Physics( numTeams );

    vector<RobotPhysics*> gRobots = physics->getAllRobots();

    for(int i = 0; i < physics->getNumTeams(); i++) {
        vector<RobotStrategy*> robotStrategiesTeam;
        for(int j = 0; j < numRobotsTeam; j++) {
            RobotStrategy* robotStrategy = new RobotStrategy( 0 );
            robotStrategiesTeam.push_back( robotStrategy );
        }
        gameState->robotStrategiesTeam = robotStrategiesTeam;
        gameState->robotStrategiesAdv = robotStrategiesTeam;
    }

    thread_physics = new thread( bind( &Simulator::runPhysics, this ));
    thread_strategies = new thread( bind( &Simulator::runStrategies, this ));
    //thread_send = new thread(bind(&Simulator::runSender, this));
    thread_receive_team1 = new thread( bind( &Simulator::runReceiveTeam1, this ));
    thread_receive_team2 = new thread( bind( &Simulator::runReceiveTeam2, this ));
    thread_receive_control = new thread( bind( &Simulator::runReceiveControl, this ));

    thread_physics->join();
    thread_strategies->join();
    //thread_send->join();
    thread_receive_team1->join();
    thread_receive_team2->join();
    thread_receive_control->join();

    report.show();
}

void Simulator::runReceiveControl(){
    vss::IControlReceiver *controlReceiver;
    controlReceiver = new vss::ControlReceiver();
    controlReceiver->createSocket();

    while(!finish_match) {
        auto control = controlReceiver->receiveControl();

        paused = control.paused;

        if(!paused && !StartPositionsHelper::useFile) {
            vector<btVector3> positions;
            vector<btScalar> orientations;

            for(int i = 0; i < control.teamYellow.size(); i++) {
                positions.push_back( btVector3( control.teamYellow[i].x, 4, control.teamYellow[i].y ));
                orientations.push_back(btScalar(degreeToRadian(control.teamYellow[i].angle)));
            }

            for(int i = 0; i < control.teamBlue.size(); i++) {
                positions.push_back( btVector3( control.teamBlue[i].x, 4, control.teamBlue[i].y ));
                orientations.push_back(btScalar(degreeToRadian(control.teamBlue[i].angle)));
            }

            physics->setRobotsPose( positions, orientations );
            physics->setBallPosition(btVector3(control.ball.x, 4, control.ball.y));
        }
    }
}

void Simulator::runReceiveTeam1(){
    vss::ICommandReceiver *commandReceiver;
    commandReceiver = new vss::CommandReceiver();
    commandReceiver->createSocket(vss::TeamType::Yellow);

    while(!finish_match) {
        auto command = commandReceiver->receiveCommand();

        if(status_team_1 == -1) {
            status_team_1 = 0;
            cout << "---Time amarelo conectado---" << endl;
        }

        for(unsigned int i = 0; i < command.commands.size() && i < 3; i++) {
            commands.at( i ) = Command(static_cast<float>(command.commands[i].leftVel + 0.001),
                                       static_cast<float>(command.commands[i].rightVel + 0.001));
        }
    }
}

void Simulator::runReceiveTeam2(){
    vss::ICommandReceiver *commandReceiver;
    commandReceiver = new vss::CommandReceiver();
    commandReceiver->createSocket(vss::TeamType::Blue);

    while(!finish_match) {
        auto command = commandReceiver->receiveCommand();

        if(status_team_2 == -1) {
            status_team_2 = 0;
            cout << "---Time azul conectado---" << endl;
        }

        for(unsigned int i = 0; i < command.commands.size() && i < 3; i++) {
            commands.at( i + 3 ) = Command(static_cast<float>(command.commands[i].leftVel + 0.001),
                                           static_cast<float>(command.commands[i].rightVel + 0.001));
        }
    }
}

void Simulator::runSender(){
    vss::State state;
    vector<RobotPhysics*> listRobots = physics->getAllRobots();

    if(report.total_of_goals_team[0] != goals_team_1) {
        goals_team_1 = report.total_of_goals_team[0];
    }

    if(report.total_of_goals_team[1] != goals_team_2) {
        goals_team_2 = report.total_of_goals_team[1];
    }

    state.ball.x = physics->getBallPosition().getX();
    state.ball.y = physics->getBallPosition().getZ();
    state.ball.speedX = physics->getBallVelocity().getX();
    state.ball.speedY = physics->getBallVelocity().getZ();

    for(int i = 0; i < 3; i++) {
        btVector3 posRobot = getRobotPosition( listRobots.at( i ));
        btVector3 velRobot = getRobotVelocity( listRobots.at( i ));
        float rads = atan2( getRobotOrientation( listRobots.at( i )).getZ(), getRobotOrientation( listRobots.at( i )).getX());

        vss::Robot robot;

        robot.x = posRobot.getX();
        robot.y = posRobot.getZ();
        robot.angle = radianToDegree(rads);
        robot.speedX = velRobot.getX();
        robot.speedY = velRobot.getZ();
        robot.speedAngle = 0;

        state.teamYellow.push_back(robot);
    }

    for(int i = 0; i < 3; i++) {
        btVector3 posRobot = getRobotPosition( listRobots.at( i + 3 ));
        btVector3 velRobot = getRobotVelocity( listRobots.at( i + 3 ));
        float rads = atan2( getRobotOrientation( listRobots.at( i + 3 )).getZ(), getRobotOrientation( listRobots.at( i + 3 )).getX());

        vss::Robot robot;

        robot.x = posRobot.getX();
        robot.y = posRobot.getZ();
        robot.angle = radianToDegree(rads);
        robot.speedX = velRobot.getX();
        robot.speedY = velRobot.getZ();
        robot.speedAngle = 0;

        state.teamBlue.push_back(robot);
    }

    stateSender->sendState(state);
}

void Simulator::runPhysics(){
    int subStep = 1;
    float standStep = 1.f / 60.f;

    arbiter.allocPhysics( physics );
    arbiter.allocReport( &report );
    arbiter.allocPaused( &paused );

    stateSender->createSocket();

    while(!finish_match) {
        if(!paused) {
            usleep( 1000000.f * timeStep / handTime );

            //physics->setBallVelocity(btVector3(0.1, 0, 0));
            loopBullet++;
            //cout << "--------Ciclo Atual:\t" << loopBullet << "--------" << endl;
            if(gameState->sameState) {
                physics->stepSimulation( timeStep, subStep, standStep );
                gameState->sameState = false;

                report.qtd_of_steps++;
            }

            updateReport();

            arbiter.checkWorld();

            if(!develop_mode) {
                if(report.total_of_goals_team[0] >= qtd_of_goals || report.total_of_goals_team[1] >= qtd_of_goals || report.qtd_of_steps > 3500 * qtd_of_goals) {
                    finish_match = true;
                }
            }

            runSender();
        }else{
            usleep( 500000 );
        }
    }
}

void Simulator::updateReport(){
    vector<BulletObject*> listRobots = physics->getAllBtRobots();

    float minDist = 9999;
    int idDist = -1;
    for(int i = 0; i < listRobots.size(); i++) {
        if(listRobots.at( i )->hitRobot) {
            if(i >= listRobots.size() / 2) {
                int id = i - listRobots.size() / 2;
                report.collisions_in_high_speed_team[1][id]++;
                //cout << "testeeeee-1" << endl;
            }else{
                report.collisions_in_high_speed_team[0][i]++;
            }
        }

        btTransform transTemp;
        listRobots.at( i )->body->getMotionState()->getWorldTransform( transTemp );

        btVector3 rbPos = transTemp.getOrigin();
        btVector3 ballPos = physics->getBallPosition();

        btVector3 dBallRbDist = rbPos - ballPos;

        btVector3 velRobot = listRobots.at( i )->body->getLinearVelocity() * timeStep;
        //cout << "traveled:\t" << velRobot.length() << endl;
        float modDist = dBallRbDist.length();

        if(i >= listRobots.size() / 2) {
            int id = i - listRobots.size() / 2;
            report.travelled_distance_team[1][id] += velRobot.length();
            //cout << "testeeeee-1" << endl;
        }else{
            report.travelled_distance_team[0][i] += velRobot.length();
        }

        if(minDist > modDist) {
            minDist = modDist;
            idDist = i;
        }
    }

    if(idDist > listRobots.size() / 2) {
        report.ball_possession_team[1]++;
    }else{
        report.ball_possession_team[0]++;
    }
}

void Simulator::runStrategies(){
    btVector3 posTargets[] = {btVector3( SIZE_WIDTH, 0, SIZE_DEPTH / 2 ), btVector3( 0, 0, SIZE_DEPTH / 2 )};
    int attackDir = 0;
    int framesSec = (int)(1 / timeStep);

    for(int i = 0; i < physics->getNumTeams(); i++) {
        if(posTargets[i].getX() > 0) attackDir = 1;
        else attackDir = -1;
        strategies[i]->setAttackDir( attackDir );
        strategies[i]->setFramesSec( framesSec );

        for(int j = 0; j < numRobotsTeam; j++) {
            int id = i * numRobotsTeam + j;
            physics->getAllRobots()[id]->setTimeStep( timeStep );
        }

    }

    while(!finish_match) {
        if(!paused) {
            usleep( 1000000.f * timeStep / handTime );

            if(!gameState->sameState) {
                updateWorld();

                if(strategies.size() > 0) {
                    btVector3 ballPos = calcRelativePosition( physics->getBallPosition(), strategies[0]->getAttackDir());
                    calcRelativeWorld( gameState->robotStrategiesTeam, strategies[0]->getAttackDir());

                    strategies[0]->runStrategy( gameState->robotStrategiesTeam, gameState->robotStrategiesTeam, ballPos );
                    if(strategies.size() == 2) {
                        btVector3 ballPos = calcRelativePosition( physics->getBallPosition(), strategies[1]->getAttackDir());
                        calcRelativeWorld( gameState->robotStrategiesAdv, strategies[1]->getAttackDir());
                        calcRelativeWorld( gameState->robotStrategiesTeam, strategies[0]->getAttackDir());
                        strategies[1]->runStrategy( gameState->robotStrategiesAdv, gameState->robotStrategiesTeam, ballPos );
                    }
                }else{
                    cout << "You must set a strategy to run the simulator!\n" << endl;
                    exit( 0 );
                }

                for(int i = 0; i < physics->getNumTeams(); i++) {
                    for(int j = 0; j < numRobotsTeam; j++) {
                        int id = i * numRobotsTeam + j;
                        if(strategies[i]->getAttackDir() == 1) {
                            //cout << id << endl;
                            float command[2] = { commands.at( id ).left, commands.at( id ).right };
                            //if(id == 0)
                            //cout << command[0] << " - " << command[1] << endl;

                            //command[1] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[1];
                            //command[0] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[0];

                            physics->getAllRobots()[id]->updateRobot( command );
                        }
                        else{
                            float invCommand[2] = { commands.at( id ).left, commands.at( id ).right };

                            //invCommand[0] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[1];
                            //invCommand[1] = strategies[i]->getRobotStrategiesTeam()[j]->getCommand()[0];

                            physics->getAllRobots()[id]->updateRobot( invCommand );
                        }
                    }
                }

                gameState->sameState = true;
            }
        }else{
            usleep( 500000 );
        }
    }
}

void Simulator::updateWorld(){
    vector<RobotPhysics*> gRobots = physics->getAllRobots();

    for(int i = 0; i < physics->getNumTeams(); i++) {
        vector<RobotStrategy*> robotStrategiesTeam;
        for(int j = 0; j < numRobotsTeam; j++) {
            RobotStrategy* robotStrategy;
            int idRobot = i * numRobotsTeam + j;
            robotStrategy = updateLocalPhysics( j, gRobots[idRobot] );
            robotStrategiesTeam.push_back( robotStrategy );
        }
        if(i == 0) gameState->robotStrategiesTeam = robotStrategiesTeam;
        else gameState->robotStrategiesAdv = robotStrategiesTeam;
    }
}

RobotStrategy* Simulator::updateLocalPhysics( int id, RobotPhysics* physicRobot ){
    RobotStrategy* robotStrategy = new RobotStrategy( id );

    btTransform transTemp;
    physicRobot->getRigidBody()->getMotionState()->getWorldTransform( transTemp );
    robotStrategy->setPosition( transTemp.getOrigin());

    btVector3 forwardVec = physicRobot->getRaycast()->getForwardVector();

    robotStrategy->setLocalFront( forwardVec );

    return robotStrategy;
}

void Simulator::calcRelativeWorld( vector<RobotStrategy*> robotStrategiesTeam, int attackDir ){
    for(unsigned int i = 0; i < robotStrategiesTeam.size(); i++) {
        robotStrategiesTeam[i]->setPosition( calcRelativePosition( robotStrategiesTeam[i]->getPosition(), attackDir ));
        robotStrategiesTeam[i]->setLocalFront( attackDir * robotStrategiesTeam[i]->getLocalFront());
        robotStrategiesTeam[i]->setLocalRight( attackDir * robotStrategiesTeam[i]->getLocalRight());
    }
}

btVector3 Simulator::calcRelativePosition( btVector3 absPos, int attackDir ){
    float relX = absPos.getX();
    //cout << attackDir;
    float relZ = absPos.getZ();
    if(attackDir == -1) {

        relZ = SIZE_DEPTH - absPos.getZ();
        relX = SIZE_WIDTH - absPos.getX();
    }
    return btVector3( relX, 0, relZ );
}

btVector3 Simulator::getRobotPosition( RobotPhysics* robot ){
    btTransform transTemp;
    robot->getRigidBody()->getMotionState()->getWorldTransform( transTemp );
    return transTemp.getOrigin();
}

btVector3 Simulator::getRobotOrientation( RobotPhysics* robot ){
    btVector3 forwardVec = robot->getRaycast()->getForwardVector();
    return forwardVec;
}

btVector3 Simulator::getRobotVelocity( RobotPhysics* robot ){
    return robot->getRigidBody()->getLinearVelocity();
}

float Simulator::radianToDegree(float radian) {
    float degree = radian * 180.0 / M_PI;

    if(degree < 0)
        degree = 360 + degree;

    return degree;
}

float Simulator::degreeToRadian(float degree) {
    if(degree > 180)
        degree = (360 - degree)*-1;

    float radian = degree * M_PI / 180.0;
    return radian;
}
