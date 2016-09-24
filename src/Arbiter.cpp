#include "Arbiter.h"

Arbiter::Arbiter(){
	refresh = false;

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
		refresh = true;
		situation = GOAL_TEAM1;
		report->total_of_goals_team[0]++;
		cerr << "---Goal RIGHT---" << endl;
		position_objects_after_goal_team_1();
	}else
	if(ball.getX() < 10){
		refresh = true;
		situation = GOAL_TEAM2;
		report->total_of_goals_team[1]++;
		cerr << "---Goal LEFT---" << endl;
		position_objects_after_goal_team_2();
	}

	vector<RobotPhysics*> listRobots = physics->getAllRobots();

	if(report->qtd_of_steps % 3500 == 0){
		cout << "---" << report->qtd_of_steps / 3500 << " MIN---" << endl;
	}

	report->qtd_of_steps++;     
 
	return situation;
}    

void Arbiter::position_objects_after_goal_team_1(){
	vector<btVector3> robots;

	robots.push_back(btVector3(55,4,45));
	robots.push_back(btVector3(35,4,30));	
	robots.push_back(btVector3(15,4,SIZE_DEPTH- 55));
	robots.push_back(btVector3(SIZE_WIDTH-55,4,85));
	robots.push_back(btVector3(SIZE_WIDTH-25,4,SIZE_DEPTH - SIZE_DEPTH/2.5 + 20));
	robots.push_back(btVector3(SIZE_WIDTH-15,4,55));

	physics->setBallPosition(btVector3( (SIZE_WIDTH/2.0)+10 , 2.0, SIZE_DEPTH/2.0));
	physics->setRobotsPosition(robots);
}

void Arbiter::position_objects_after_goal_team_2(){
	vector<btVector3> robots;
	
	robots.push_back(btVector3(55,4,45));
	robots.push_back(btVector3(35,4,30));	
	robots.push_back(btVector3(15,4,SIZE_DEPTH- 55));
	robots.push_back(btVector3(SIZE_WIDTH-55,4,85));
	robots.push_back(btVector3(SIZE_WIDTH-25,4,SIZE_DEPTH - SIZE_DEPTH/2.5 + 20));
	robots.push_back(btVector3(SIZE_WIDTH-15,4,55));

	
	physics->setBallPosition(btVector3( (SIZE_WIDTH/2.0)+10 , 2.0, SIZE_DEPTH/2.0));
	physics->setRobotsPosition(robots);
}