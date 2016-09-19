#include "Arbiter.h"

Arbiter::Arbiter(){

}

int Arbiter::checkWorld(vss_state::Global_State global_state){
	if(global_state.balls_size() > 0){
		if(global_state.balls(0).pose().x() > 160){
			cerr << "Goal Team 1" << endl;
			return GOAL_TEAM1;
		}else
		if(global_state.balls(0).pose().x() < 10){
			cerr << "Goal Team 2" << endl;
			return GOAL_TEAM2;
		}else{
			return NONE;
		}
	}else{
		return NONE;
	}
}