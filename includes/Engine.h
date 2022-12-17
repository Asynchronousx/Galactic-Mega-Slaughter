#pragma once
#include "Player.h"
#include "Drawer.h"
#include "Object.h"
#include "Weapon.h"
#include "EnemyManager.h"
#include "CollisionHandler.h"
#include "ConsumablesManager.h"
#include "SpawnHandler.h"

enum class IDLEFUNC {IDLE, AIM, SHOOT, WALK, RELOAD, SWAP, NONE};

class Engine {

	public:

		// Attributes
		Drawer drawer;
		Player player;
		CollisionHandler collisionHandler;
		SpawnHandler spawnHandler;
		EnemyManager enemyManager;
		ConsumablesManager consumableManager;

		// Constructor/Destructor
		Engine() {}
		~Engine() {}

		// Functions
		void setup(void);
		void update(void);
		void resize(int, int);
		void render(void);
		void activeLook(int, int);
		void passiveLook(int, int);
		IDLEFUNC aim(int);
		IDLEFUNC shoot(int);
		IDLEFUNC swapWeapon(void);
		IDLEFUNC performActionOnPress(unsigned char);
		IDLEFUNC performActionOnRelease(unsigned char);

	private:

		// Attributes
		Object environment;

};