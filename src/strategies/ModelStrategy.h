#ifndef ModelStrategy_H_
#define ModelStrategy_H_

#include "Header.h"
#include "RobotStrategy.h"

#define TURN_ROBOT 2
#define SIMULATOR_TO_VSS 1

class ModelStrategy{
protected:
    clock_t begin_time;
    float deltaTime;
    int numFramesPerSec;

    int attackDir;

	btVector3 ballPos;
	btVector3 ballVel;
	btVector3 ballAce;
	btVector3 posGoalpost;
	float timeStep;

    vector<RobotStrategy*> robotStrategiesTeam;
	vector<RobotStrategy*> robotStrategiesAdv;
	vector<ModelStrategy*> strategyHistory;

	virtual void updateStrategiesHistory();

    virtual void updateTargetPosition(RobotStrategy* robotStrategy);

    virtual void updateDynamics();
    virtual void updateLocalDynamics(RobotStrategy* cur, RobotStrategy* old);

public:
    ModelStrategy();
    void runStrategy();
	virtual void runStrategy(vector<RobotStrategy*> robotStrategiesTeam, vector<RobotStrategy*> robotStrategiesAdv, btVector3 ballPos) = 0;
	virtual void setAttackDir(int goalpostDir);
	int getAttackDir(){ return attackDir; }
    virtual void setFramesSec(int frames);

	virtual vector<ModelStrategy*> getStrategyHistory(){ return strategyHistory; }

	virtual vector<RobotStrategy*> getRobotStrategiesTeam(){ return robotStrategiesTeam; }
	virtual vector<RobotStrategy*> getRobotStrategiesAdv(){ return robotStrategiesAdv; }

	btVector3 getBallPosition(){ return ballPos; }
	btVector3 getBallVelocity(){ return ballVel; }
	btVector3 getBallAcceleration(){ return ballAce; }

	float getAttackGoal(){ return attackDir; }
};

#endif
