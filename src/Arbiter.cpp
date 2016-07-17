#include "Arbiter.h"

Arbiter::Arbiter(){

}
	
void Arbiter::allocateState(vss_state::Global_State *global_state){
	this->global_state = global_state;
}

int Arbiter::checkWorld(){
	if(global_state->balls(0).pose().x() > 150){
		cerr << "Goal Team 1" << endl;
		return GOAL_TEAM1;
	}else
	if(global_state->balls(0).pose().x() < 0){
		cerr << "Goal Team 2" << endl;
		return GOAL_TEAM2;
	}else{
		//cerr << "None" << endl;
		return NONE;
	}
}