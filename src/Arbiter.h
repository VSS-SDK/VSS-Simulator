#ifndef _ARBITER_H_
#define _ARBITER_H_

#include "iostream"
#include "VSS-Interface/interface.h"
#include "Sir.h"

using namespace std; 

class Arbiter{
public:
	Arbiter();

	int checkWorld(vss_state::Global_State);
};

#endif // _ARBITER_H_