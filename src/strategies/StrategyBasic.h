#ifndef STRATEGYBASIC_H_
#define STRATEGYBASIC_H_

#include "Header.h"
#include "ModelStrategy.h"

class StrategyBasic : public ModelStrategy{
private:
    struct LocalFunc{ enum fuction {goal, deffense, attack}; };

    float areaTarget = 10.f;
    float distProjBallWall = 5.f;
    float maxDiffRobotPos = 5.f;
    int timeToFullHistory;

	float timeStep;

	float framesStopped;
	float framesBackward;

    void updateStrategiesHistory();
    void updateFunctions();

	void controlLocalStrategy(RobotStrategy* robotStrategy);
	void updateTargetPosition(RobotStrategy* robotStrategy);
	bool correctWallTargetPosition(RobotStrategy* robotStrategy);
	void updateLocalFuncAction(RobotStrategy* robotStrategy);
	void handleLocalActions(RobotStrategy* robotStrategy);

	//Actions
	void updateActionShoot(RobotStrategy* robotStrategy);
	void updateActionMotion(RobotStrategy* robotStrategy);
	bool updateActionStopped(RobotStrategy* robotstrategy);

	//Functions
	void updateGoalStrategyAction(RobotStrategy* robotStrategy);
	void updateDeffenseStrategyAction(RobotStrategy* robotStrategy);
	void updateAttackStrategyAction(RobotStrategy* robotStrategy);

	float handleLocalMaxVelocity(RobotStrategy* robotStrategy);
public:
	StrategyBasic();
	~StrategyBasic();

    float getAttackGoal() { return posGoalpost.getX(); }

    vector<ModelStrategy*> getStrategyHistory() { return strategyHistory; }

	void runStrategy(vector<RobotStrategy*> robotStrategiesTeam,vector<RobotStrategy*> robotStrategiesAdv,btVector3 ballPos);
	vector<RobotStrategy*> getRobotStrategiesTeam() { return robotStrategiesTeam; }
	vector<RobotStrategy*> getRobotStrategiesAdv() { return robotStrategiesAdv; }

};

#endif
