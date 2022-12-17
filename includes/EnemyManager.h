#pragma once
#include "Enemy.h"

// Class that will perform various action on the enemy class, managing his resource and logic
class EnemyManager {

	public:

		// Attributes
		GAMEMODE difficulty;
		std::vector<Enemy> Enemies;
		std::vector<std::pair<std::unique_ptr<Enemy>, std::unique_ptr<Enemy>>> ProbeTemplates;

		int tick;
		int soundTick;
		int totalkill;

		// Constructor
		EnemyManager(GAMEMODE difficulty=GAMEMODE::NORMAL) {
			this->init();			
		}
		

		// Destructor 
		~EnemyManager() {}

		// Functions 
		void manageEnemies(std::vector<float>);
		void moveToPlayer(std::vector<float>);
		void checkDeath(void);
		void updateTick(void);
		void assignTemplates(void);
		void loadEnemy(void);
		void reset(void);

	private:

		// Attributes
		bool templateLoaded;

		// Functions
		void init(void);



};