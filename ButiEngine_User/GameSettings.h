#pragma once

class GameSettings
{
public:
	~GameSettings(){}
	static const float BLOCK_SIZE;
	static const std::uint16_t MAP_CHIP_PLAYER = 1;
	static const std::uint16_t MAP_CHIP_BLOCK = 2;
	static const std::uint16_t MAP_CHIP_GLASS = 3;
	static const std::uint16_t MAP_CHIP_TUTORIALGOAL = 4;
	static const std::uint16_t MAP_CHIP_EASYGOAL = 5;
	static const std::uint16_t MAP_CHIP_DEFAULTGOAL = 6;
	static const std::uint16_t MAP_CHIP_PLAYER_ROTATE_90 = 7;
	static const std::uint16_t MAP_CHIP_PLAYER_ROTATE_180 = 8;
	static const std::uint16_t MAP_CHIP_PLAYER_ROTATE_MIN_90 = 9;

	static const std::uint16_t MAP_CHIP_INVISIBLEBLOCK = 100;
	static const std::uint16_t MAP_CHIP_PLAYER_AND_GOAL = 200;

	static const std::uint16_t MAP_CHIP_NEXT_STAGE_BLOCK = 300;


	static const ButiEngine::Vector4 ACTIVE_GOAL_COLOR;
	static const ButiEngine::Vector4 INACTIVE_GOAL_COLOR;

private:
	GameSettings(){}
};

