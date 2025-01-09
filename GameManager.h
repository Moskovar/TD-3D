#pragma once
#include "Player.h"

class GameManager
{
	public:
		GameManager() {}
		GameManager(Player* rPlayer, Player* lPlayer, float* deltaTime);
		~GameManager();

		void manage();

	private:
		Player* rPlayer  = nullptr, * lPlayer = nullptr;
		float* deltaTime = nullptr;

		void manageTower();
		void manageUnits();
};

