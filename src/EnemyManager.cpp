#include "EnemyManager.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that moves each enemy closer to the player 
void EnemyManager::moveToPlayer(std::vector<float> positions) {

	// If there are enemies
	if (Enemies.size() > 0) {

		// for each enemy 
		for (int i = 0; i < this->Enemies.size(); i++) {

			// Move them to the player 
			this->Enemies.at(i).move(positions.at(0), positions.at(1), positions.at(2));

		}

	}

}
// Function that does control in the enemies vector which one has 0 hp, then discard it
void EnemyManager::checkDeath(void) {

	// We can use two methods here: with erase and popback.
	// While with erase we can delete multiple enemies at time, i encountered some out of range
	// error and idk why. Probably because of how enemies are pushed in tbh, but couldn't fix it
	
	// Vector that store the index of the dead enemies -- ERASE METHOD
	//std::vector<int> deadEnemies;

	// There is any enemy?
	if (Enemies.size() > 0) {

		// If so, for each enemy 
		for (int i = 0; i < this->Enemies.size(); i++) {

			// Check his life 
			if (this->Enemies.at(i).hp <= 0) {

				// If the hps are lessed than zero, we swap back the current enemy and we pop it.
				// Why using this method? Because it's safer than erase that may lead in some 
				// out of range erros
				// POP BACK METHOD
				// 
				// if the enemy vector is > 1, then swap the cur element into the back (no sense swapping an elem with itself)
				if (this->Enemies.size() > 1) std::swap(Enemies.at(i), Enemies.back());

				// Fetch scale before popping it to use it as multiplied for the score (the bigger the better :p)
				float scaleMultiplier = this->Enemies.back().scale;

				// Play death sound before popping it
				this->Enemies.at(i).deathSound();

				// pop it
				this->Enemies.pop_back();
				
				// ERASE METHOD
				// deadEnemies.push_back(i);	
				
				// increse kill score
				this->totalkill += round(1*scaleMultiplier * 10);

			}

		}

		/* ERASE METHOD
		// If an enemy was killed 
		if (deadEnemies.size() > 0) {

			// Delete every index in which the hp were lesser than 0
			for (int j = 0; j < deadEnemies.size(); j++) {
				this->Enemies.erase(this->Enemies.begin() + deadEnemies.at(j));
			}

		}
		*/

	}

}

// Function that increase the internal tick counter to take track of time elapsed
void EnemyManager::updateTick(void) {

	// Based on the difficulty choosen, we update the tick accordingly
	switch (this->difficulty) {

	case GAMEMODE::EASY:
		if (this->tick > 360.0f) this->tick = 1;
		else this->tick++;
		break;

	case GAMEMODE::NORMAL:
		if (this->tick > 240.0f) this->tick = 1;
		else this->tick++;
		break;

	case GAMEMODE::HARD:
		if (this->tick > 120.0f) this->tick = 1;
		else this->tick++;
		break;

	case GAMEMODE::MADNESS:
		if (this->tick > 60.0f) this->tick = 1;
		else this->tick++;
		break;

	}

	// Manage sound tick
	if (this->soundTick > 60.0f) {
		this->soundTick = 0;
	}
	else {
		this->soundTick++;
	}

}

// Function that given a preloaded enemy will load it into an apposite structure
void EnemyManager::assignTemplates(void) {

	// if we haven't loaded the templates yet (i.e: first game)
	if (!this->templateLoaded) {
		// RED PROBE
		// Declaring two enemies, hit and normal
		Enemy* rnormal = new Enemy(IDENTIFIER::RED);
		Enemy* hit = new Enemy(IDENTIFIER::RED);

		// Load their assets with respective colors
		rnormal->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/enemies/probe/red/", "probe.obj", "probe.mtl");
		hit->loadAsset(Color4f(1.0, 0.0, 0.0, 1.0), "resources/models/enemies/probe/red/", "probe.obj", "probe.mtl");


		this->ProbeTemplates.emplace_back(std::make_pair(rnormal, hit));

		// BLUE
		// Declaring blue probe
		Enemy* bnormal = new Enemy(IDENTIFIER::BLUE);

		// Load their assets with respective colors
		bnormal->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/enemies/probe/blue/", "probe.obj", "probe.mtl");

		// Emplace back
		this->ProbeTemplates.emplace_back(std::make_pair(bnormal, hit));

		// BRONZE
		// Declaring bronze probe
		Enemy* brnormal = new Enemy(IDENTIFIER::BRONZE);

		// Load their assets with respective colors
		brnormal->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/enemies/probe/blackbronze/", "probe.obj", "probe.mtl");

		// Emplace back
		this->ProbeTemplates.emplace_back(std::make_pair(brnormal, hit));

		// BLOOD
		// Declaring blood probe
		Enemy* blnormal = new Enemy(IDENTIFIER::BLOOD);

		// Load their assets with respective colors
		blnormal->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/enemies/probe/blackred/", "probe.obj", "probe.mtl");

		// Emplace back
		this->ProbeTemplates.emplace_back(std::make_pair(blnormal, hit));

		// ORANGE
		// Declaring orange probe
		Enemy* onormal = new Enemy(IDENTIFIER::ORANGE);

		// Load their assets with respective colors
		onormal->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/enemies/probe/orange", "probe.obj", "probe.mtl");

		// Emplace back
		this->ProbeTemplates.emplace_back(std::make_pair(onormal, hit));

		// Flag the loaded templates
		this->templateLoaded = true;

	}

}

// Function that performs some action into the engine (moving, check deaths)
void EnemyManager::manageEnemies(std::vector<float> positions) {

	this->checkDeath();
	this->moveToPlayer(positions);

}

// Function that load the enemy assets in the model
void EnemyManager::loadEnemy(void) {

	// Choose a random enemy from the enemy identifier template 
	int randModel = rand() % 4 + 0;

	// Create a new enemy, randomize attributes and push it back
	// We do assign to the default constructor a random identifier casting the random number
	/// between 0/4 to the ENUM IDENTIFIER.
	Enemy newenemy = Enemy((IDENTIFIER)randModel);
	newenemy.randomize(this->difficulty);

	// Play hoover sound 
	newenemy.hoverSound();

	// push enemy back
	this->Enemies.emplace_back(newenemy);


}

// Function that reset elements when a new game starts
void EnemyManager::reset(void) {

	this->Enemies.clear();
	this->tick = 0;
	this->soundTick = 0;
	this->totalkill = 0;

}

///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

void EnemyManager::init(void) {

	this->templateLoaded = false;
	this->difficulty = difficulty;
	this->tick = 0;
	this->soundTick = 0;
	this->totalkill = 0;	

}
