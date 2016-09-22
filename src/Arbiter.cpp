#include "Arbiter.h"

Arbiter::Arbiter(){

}

void Arbiter::allocPhysics(Physics *physics){
	this->physics = physics;
}

int Arbiter::checkWorld(){
	int situation = NONE;
	btVector3 ball = physics->getBallPosition();
	if(ball.getX() > 160){
		situation = GOAL_TEAM1;
		cerr << "---Goal Team 1---" << endl;
		position_objects_after_goal_team_1();
	}else
	if(ball.getX() < 10){
		situation = GOAL_TEAM2;
		cerr << "---Goal Team 2---" << endl;
		position_objects_after_goal_team_2();
	}
	

	return situation;
}    

void Arbiter::position_objects_after_goal_team_1(){
	vector<btVector3> robots;

	for(int i = 0 ; i < 6 ; i++){
		robots.push_back(btVector3(i*20, 80, 0));
	}

	physics->setBallPosition(btVector3( (SIZE_WIDTH/2.0)+10 , 2.0, SIZE_DEPTH/2.0));
	physics->setRobotsPosition(robots);
}

void Arbiter::position_objects_after_goal_team_2(){
	vector<btVector3> robots;
	
	for(int i = 0 ; i < 6 ; i++){
		robots.push_back(btVector3(i*20, 80, 0));
	}
	
	physics->setBallPosition(btVector3( (SIZE_WIDTH/2.0)+10 , 2.0, SIZE_DEPTH/2.0));
	physics->setRobotsPosition(robots);
}