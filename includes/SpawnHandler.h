#pragma once
#include "RNG.h"
#include "Utils.h"
#include "EnemyManager.h"
#include "ConsumablesManager.h"

// Class that manages the spawn of various entity across the world 
class SpawnHandler {

	public:

		// Attributes 
		RNG rng;
		EnemyManager* enemyManager;
		ConsumablesManager* consumableManager;

		// Constructor/Destructor 
		SpawnHandler() {}
		~SpawnHandler() {}
		
		// Functions 
		void spawnEnemy(void);
		void spawnConsumable(Vector3f, int);

};