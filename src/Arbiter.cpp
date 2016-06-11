#include "Arbiter.h"

Arbiter::Arbiter(){

}
	
void Arbiter::allocateState(vss_state::Global_State *global_state){
	this->global_state = global_state;
}

int Arbiter::checkWorld(){
	if(false){
		return GOAL;
	}else{
		return NONE;
	}
}