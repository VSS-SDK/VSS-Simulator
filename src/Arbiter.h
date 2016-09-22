#ifndef _ARBITER_H_
#define _ARBITER_H_

#include "iostream"
#include "VSS-Interface/interface.h"
#include "Sir.h"
#include "Physics.h"

using namespace std; 

class Arbiter{
protected:
	Physics *physics;
public:
	Arbiter();

	void allocPhysics(Physics*);
	int checkWorld();

	void position_objects_after_goal_team_1();
	void position_objects_after_goal_team_2();
};

#endif // _ARBITER_H_