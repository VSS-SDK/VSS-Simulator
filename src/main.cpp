#include "Header.h"
#include "Simulator.h"
#include "strategies/ModelStrategy.h"
#include "strategies/Strategy.h"
#include "strategies/StrategyBasic.h"


//Exemplo de estratégia
class StrategyTest : public ModelStrategy{
public:
    void runStrategy(vector<RobotStrategy*> robotStrategiesTeam,vector<RobotStrategy*> robotStrategiesAdv,btVector3 ballPos){
        this->robotStrategiesTeam = robotStrategiesTeam;
        this->robotStrategiesAdv = robotStrategiesAdv;
        this->ballPos = ballPos;

        ModelStrategy::runStrategy();

        for(int i = 0; i < robotStrategiesTeam.size(); i++){
            float leftWheel, rigthWheel;
            leftWheel = 0;
            rigthWheel = 0;
            robotStrategiesTeam[i]->updateCommand(leftWheel,rigthWheel);
        }
    }
};

int main(int argc, char *argv[]){
	Strategy *stratYellowTeam = new Strategy(); //Original strategy
	Strategy *stratBlueTeam = new Strategy(); //Strategy for tests

	Simulator* simulator = new Simulator();
	simulator->runSimulator(argc, argv, stratBlueTeam,stratYellowTeam);

	return 0;
}
