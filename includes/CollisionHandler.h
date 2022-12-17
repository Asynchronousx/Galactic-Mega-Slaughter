#pragma once
#include "Utils.h"
#include "Player.h"
#include "EnemyManager.h"
#include "SpawnHandler.h"

class CollisionHandler {

	public:

		// Attributes
		Utils utils;
		Player* player;
		SpawnHandler* handler;

		// Constructor/Destructor
		CollisionHandler() {}
		~CollisionHandler() {}

		// Functions 
		void checkCollisions(void);


	private:

		// Attributes

		// Functions
		void checkEnemyShot(void);
		void checkPlayerHit(void);
		void checkConsumableCollision(void);
		void checkBoundaryCollision(void);

};
