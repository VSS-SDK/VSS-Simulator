#ifndef STRATEGY_H_
#define STRATEGY_H_

#define SIMULATOR_TO_VSS 1.0
#define TURN_ROBOT 1.0

#include "Header.h"
#include "ModelStrategy.h"

class Strategy : public ModelStrategy{
private:
    struct LocalFunc{ enum fuction {goal, deffense, attack}; };

    float sizeRobot;
    float areaTarget;
    float distProjBallWall;
    float maxDiffRobotPos;
    int timeToFullHistory;

    btVector3 ballTarget;

	float timeStep;

	float framesStopped[3];
	float framesBackward[3];

	void calcBallTarget();

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
	void updateVerticalAlign(RobotStrategy* robotStrategy);
	void updateHorizontalAlign(RobotStrategy* robotStrategy);
	bool updateActionStopped(RobotStrategy* robotstrategy);

	//Functions
	void updateGoalStrategyAction(RobotStrategy* robotStrategy);
	void updateDeffenseStrategyAction(RobotStrategy* robotStrategy);
	void updateAttackStrategyAction(RobotStrategy* robotStrategy);

	//Exceptions
	bool updateTwistAction(RobotStrategy* robotStrategy);
	bool verifyDeffenseTarget(RobotStrategy* robotStrategy);

	float handleLocalMaxVelocity(RobotStrategy* robotStrategy);
public:
	Strategy();
	~Strategy();

	void runStrategy(vector<RobotStrategy*> robotStrategiesTeam,vector<RobotStrategy*> robotStrategiesAdv,btVector3 ballPos);

};

#endif
