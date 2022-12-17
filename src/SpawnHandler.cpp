#include "SpawnHandler.h"

// Function that, based on the game modality will spawn enemies more or less frequently
void SpawnHandler::spawnEnemy(void) {

	// Switching game modality 
	switch (this->enemyManager->difficulty) {

		// Based on the difficulty, we do push back a new enemies once every 6/4/2 sec
	case GAMEMODE::EASY:
		if (!(this->enemyManager->tick % 360)) this->enemyManager->loadEnemy();
		break;

	case GAMEMODE::NORMAL:
		if (!(this->enemyManager->tick % 240)) this->enemyManager->loadEnemy();
		break;

	case GAMEMODE::HARD:
		if (!(this->enemyManager->tick % 120)) this->enemyManager->loadEnemy();
		break;

	case GAMEMODE::MADNESS:
		if (!(this->enemyManager->tick % 60)) this->enemyManager->loadEnemy();
		break;

	}

}

// Function that decides or not to spawn a consumable on the ground
void SpawnHandler::spawnConsumable(Vector3f position, int multiplier) {

	// Decide to spawn or not the consumable
	if (this->rng.getRngBool(0, 10)) {

		// If we need to spawn a consumable, choose randomly between an ammobox and healthbox 
		// NOTE: if we'd like to amplify our consumables, we'll just replace getRngBool with getRng 
		// between 0 and the number of consumables class present in our game
		int decision = int(this->rng.getRngBool(0, 10));

		std::cout << "Into the spawner, decied to spawn: " << decision << std::endl;

		switch (decision) {

			// Case 0 is our healthbox
			case 0:
				position.y = 1;
				this->consumableManager->spawnHealthbox(position, 0.01, -135, multiplier);
				break;

			/// Caase 1 is our ammobox
			case 1:
				position.y = 1;
				this->consumableManager->spawnAmmobox(position, 0.1, 45, multiplier);
				break;

		}

	}

}