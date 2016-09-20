#include "Arbiter.h"

Arbiter::Arbiter(){

}

int Arbiter::checkWorld(btVector3 ball){
	if(ball.getX() > 160){
		cerr << "Goal Team 1" << endl;
		return GOAL_TEAM1;
	}else
	if(ball.getX() < 10){
		cerr << "Goal Team 2" << endl;
		return GOAL_TEAM2;
	}else{
		return NONE;
	}
}