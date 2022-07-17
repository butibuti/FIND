#pragma once

class GameSettings
{
public:
	~GameSettings(){}
	static float BlockSize;
	static const int player=1;
	static const int block=2;
	static const int tutorialGoal=3;
	static const int easyGoal=4;
	static const int defaultGoal=5;
	static const int playerRotate_90 = 6;
	static const int playerRotate_180 = 7;
	static const int playerRotate_min90 = 8;
	static const int invisibleBlock = 100;
	static const int playerAndGoal=200;

	static bool IsBlock(int i);

	static bool TriggerDecision();
	static bool CheckRight();
	static bool CheckLeft();
	static bool CheckFront();
	static bool CheckBack();

private:
	GameSettings(){}
};

