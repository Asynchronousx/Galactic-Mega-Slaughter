#include "CollisionHandler.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

void CollisionHandler::checkCollisions(void) {

	this->checkEnemyShot();
	this->checkPlayerHit();
	this->checkConsumableCollision();
	this->checkBoundaryCollision();

}


// Function that needs to check on raycasting collision of the player when 
void CollisionHandler::checkEnemyShot(void) {

	// Get the x,y,z position of the raycast: the raycast is already done 
	// when the player click the mouse. 
	// We base the enemy shot collision based on the fire rate of the weapon. 
	if (!this->player->currate && this->player->shootstate) {
		float x_ray = this->player->raycaster.x;
		float y_ray = this->player->raycaster.y;
		float z_ray = this->player->raycaster.z;

		// Now, for each enemy chekck if the ray collide with their position with a given offset
		// to simulate a range of ray collision, if any 
		if (this->handler->enemyManager->Enemies.size() > 0) {

			// For each enemy 
			for (int i = 0; i < this->handler->enemyManager->Enemies.size(); i++) {

				// Get posx and posz of the enemy
				float enemy_posx = this->handler->enemyManager->Enemies.at(i).posx;
				float enemy_posy = this->handler->enemyManager->Enemies.at(i).posy;
				float enemy_posz = this->handler->enemyManager->Enemies.at(i).posz;

				// If the player have enough ammo:
				if (this->player->inventory.weapons.at(player->currentweapon).ammo > 0) {

					if (this->utils.inRange(x_ray, enemy_posx, 5) && utils.inRange(z_ray, enemy_posz, 5) && utils.inRange(y_ray, enemy_posy, 5)) {

						// Get weapon damage			
						int dmg = this->player->inventory.weapons.at(this->player->currentweapon).dmg;



						// Check which type of weapon we do have currently equipped: if we're wearing a shotgun, 
						// we need to decrease the damage based on the distance between the player and the enemy.
						// The more the distance, the lesser the damage
						// We get the current position of the player
						if (this->player->inventory.weapons.at(this->player->currentweapon).type == TYPE::SHOTGUN) {
							float player_posx = this->player->view.eyex;
							float player_posy = this->player->view.eyey;
							float player_posz = this->player->view.eyez;

							if (player_posx - enemy_posx > 7.0f || player_posz - enemy_posz > 7.0f) {

								dmg = (int)round(dmg / (10));

							}

						}

						// Subtract damage from the enemy hp
						this->handler->enemyManager->Enemies.at(i).takeDamage(dmg, this->handler->enemyManager->soundTick);

						// Check if the enemy hp is lesser than 0 (enemy killed): if so, choose to spawn (or not)
						// a consumable item 
						if (this->handler->enemyManager->Enemies.at(i).hp <= 0) {

							// call the spawn function 
							int multiplier = round(1 * this->handler->enemyManager->Enemies.at(i).scale * 10);
							this->handler->spawnConsumable(Vector3f(enemy_posx, 0, enemy_posz), multiplier);

						}

						// Decrease the enemy position to simulate a fallback due to being shot
						int sign = this->handler->enemyManager->Enemies.at(i).posz > 0 ? -1 : 1;
						this->handler->enemyManager->Enemies.at(i).posz -= 0.2 * sign * this->handler->enemyManager->Enemies.at(i).scale;

						// Mark the enemy for being hitted
						this->handler->enemyManager->Enemies.at(i).beingShot = true;

					}
					else {

						float posxx = enemy_posx;
						float posyy = enemy_posy;
						float poszz = enemy_posz;
						float rayxx = x_ray;
						float rayyy = y_ray;
						float rayzz = z_ray;
						bool test = false;

					}
				
				}

			}

		}
	}

}


// Function that checks if some enemy reached the player
void CollisionHandler::checkPlayerHit(void) {

	// We get the current position of the player
	float player_posx = this->player->view.eyex;
	float player_posy = this->player->view.eyey;
	float player_posz = this->player->view.eyez;

	// Now, for each enemy check if their position on the map is the same as the player
	// (if some enemy present)
	if (this->handler->enemyManager->Enemies.size() > 0) {

		// For each enemy 
		for (int i = 0; i < this->handler->enemyManager->Enemies.size(); i++) {

			// Get posx and posz of the enemy
			float enemy_posx = this->handler->enemyManager->Enemies.at(i).posx;
			float enemy_posy = this->handler->enemyManager->Enemies.at(i).posy;
			float enemy_posz = this->handler->enemyManager->Enemies.at(i).posz;


			// And now check if they collide with the player using inRange utils. 
			// We do assert that the enemies hp are major than 0, because this is something that 
			// the enemy enemyManager does in the loop.
			if (this->utils.inRange(player_posx, enemy_posx, 5) && this->utils.inRange(player_posy, enemy_posy, 5) && this->utils.inRange(player_posz, enemy_posz, 5)) {

				// Get enemy power 		
				float dmg = this->handler->enemyManager->Enemies.at(i).power;

				// Subtract damage from the enemy hp
				this->player->takeDamage(dmg);

				// Decrease the player position to simulate a fallback due to being hit
				this->player->view.eyez -= 0.5;

				// Mark the player for being hitted
				this->player->isHit = true;

			}

		}


	}

}

// Function that check if the player is in range with a consumable
void CollisionHandler::checkConsumableCollision(void) {

	// Getting player pos
	float player_posx = this->player->view.eyex;
	float player_posy = this->player->view.eyey;
	float player_posz = this->player->view.eyez;

	// We gonna iterte through the consumables vector of the relative manager 
	// First one: health boxes, check if there are any 
	if (this->handler->consumableManager->healthboxes.size() > 0) {

		// if so, for every healthbox
		for (int i = 0; i < this->handler->consumableManager->healthboxes.size(); i++) {

			// Getting item pos
			float health_posx = this->handler->consumableManager->healthboxes.at(i).x;
			float health_posy = this->handler->consumableManager->healthboxes.at(i).y;
			float health_posz = this->handler->consumableManager->healthboxes.at(i).z;

			// If the position collide with the player 
			if (this->utils.inRange(player_posx, health_posx, 2) && this->utils.inRange(player_posz, health_posz, 2)) {

				// If we need to heal 
				if (this->player->hp < 100) {

					// Add the quantity to the player 
					this->player->lootHealth(this->handler->consumableManager->healthboxes.at(i).amount);

					// Remove the healthbox from the ground 
					int id = this->handler->consumableManager->healthboxes.at(i).id;
					this->handler->consumableManager->deleteHealthbox(id);

					// Play pickup health soun

				}

			}

		}

	}
	
	// I should write a function to avoid code redundancy but im bored and i wont lol
	// We gonna iterte through the consumables vector of the relative manager 
	// second one: ammo boxes, check if there are any 
	if (this->handler->consumableManager->ammoboxes.size() > 0) {

		// if so, for every healthbox
		for (int i = 0; i < this->handler->consumableManager->ammoboxes.size(); i++) {

			// Getting item pos
			float ammo_posx = this->handler->consumableManager->ammoboxes.at(i).x;
			float ammo_posy = this->handler->consumableManager->ammoboxes.at(i).y;
			float ammo_posz = this->handler->consumableManager->ammoboxes.at(i).z;

			// If the position collide with the player 
			if (this->utils.inRange(player_posx, ammo_posx, 2) && this->utils.inRange(player_posz, ammo_posz, 2)) {

				// Add the quantity to the player 
				this->player->lootAmmo(this->handler->consumableManager->ammoboxes.at(i).amount);

				// Remove the healthbox from the ground 
				int id = this->handler->consumableManager->ammoboxes.at(i).id;
				this->handler->consumableManager->deleteAmmobox(id);

			}

		}

	}

}

// Function that check the collision boundary with the world limit 
void CollisionHandler::checkBoundaryCollision(void) {

	// We first check for world limit collision:	
	// To do so, we check if the player is going far from the limit on the world 
	float player_posx = this->player->view.eyex;
	float player_posz = this->player->view.eyez;
	int posx_sign = player_posx > 0 ? 1 : -1;
	int posz_sign = player_posz > 0 ? 1 : -1;

	// Now, if the player collides with the maximum x plane 
	if (this->utils.inRange(abs(player_posx), 100, 2)) {
		this->player->view.eyex -= 2 * posx_sign;
	}

	// Now, if the player collides with the maximum z plane 
	if (this->utils.inRange(abs(player_posz), 100, 2)) {
		this->player->view.eyez -= 2 * posz_sign;
	}


}

