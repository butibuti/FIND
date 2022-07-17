#pragma once

class GameSettings
{
public:
	~GameSettings(){}
	static const float BLOCK_SIZE;
	static const std::uint8_t MAP_CHIP_PLAYER = 1;
	static const std::uint8_t MAP_CHIP_BLOCK = 2;
	static const std::uint8_t MAP_CHIP_TUTORIALGOAL = 3;
	static const std::uint8_t MAP_CHIP_EASYGOAL = 4;
	static const std::uint8_t MAP_CHIP_DEFAULTGOAL = 5;
	static const std::uint8_t MAP_CHIP_PLAYER_ROTATE_90 = 6;
	static const std::uint8_t MAP_CHIP_PLAYER_ROTATE_180 = 7;
	static const std::uint8_t MAP_CHIP_PLAYER_ROTATE_MIN_90 = 8;
	static const std::uint8_t MAP_CHIP_INVISIBLEBLOCK = 100;
	static const std::uint8_t MAP_CHIP_PLAYER_AND_GOAL = 200;

private:
	GameSettings(){}
};

