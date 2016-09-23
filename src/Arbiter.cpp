#include "Arbiter.h"

Arbiter::Arbiter(){

}

void Arbiter::allocPhysics(Physics *physics){
	this->physics = physics;
}

void Arbiter::allocReport(Report *report){
	this->report = report;
}

int Arbiter::checkWorld(){
	int situation = NONE;
	btVector3 ball = physics->getBallPosition();

	if(ball.getX() > 160){
		situation = GOAL_TEAM1;
		report->total_of_goals_team[0]++;
		cerr << "---Goal Team 1---" << endl;
		position_objects_after_goal_team_1();
	}else
	if(ball.getX() < 10){
		situation = GOAL_TEAM2;
		report->total_of_goals_team[1]++;
		cerr << "---Goal Team 2---" << endl;
		position_objects_after_goal_team_2();
	}

	vector<RobotPhysics*> listRobots = physics->getAllRobots();
	/*float dist_min = 1024;
	int id_min = 0;
	for(int i = 0 ; i < listRobots.size() ; i++){
		/*if(ball - listRobots.at(i)->getPosition()){

		}
		cout << ball - listRobots.at(i)->getPosition() << endl;
	}*/

	report->qtd_of_steps++;        
 
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