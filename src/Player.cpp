#include "Player.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that will perform a shoot action updating weapon values and performing 
// some animation


// Function that will perform a raycast operation based on the crosshair position (middle screen)
void Player::raycast(void) {

	// If the raycast is for shooting an enemy
	if (this->shootstate && !this->inventory.weapons.at(this->currentweapon).singleShotWeaponFired) {
		
		// If we have enough ammo 
		if (this->inventory.weapons.at(this->currentweapon).ammo > 0) {

			// Then perform a rayast
			this->raycaster.project(this->view.width / 2, this->view.height / 2);

		}

	}


}

// Function that finally update the camera
void Player::updateView(void) {

	this->view.update();

}

// Function that given the current location of the mouse will update the camera 
void Player::look(int x, int y) {

	this->view.update_centers(x, y);

}

// Function that given the direction will move the camera in the specified way
void Player::changePosition(DIRECTION direction) {

	// Decrease/increese the speed based on the player state
	if (this->aimstate || this->shootstate || this->reloadstate) this->view.decrease_speed();
	else this->view.restore_speed();

	// Compute new direction based on the walking dir 
	this->view.compute_directions(direction);

}

// Function that changes the position of a weapon and perform a walk, rotating the weapon 
// Simulating a walking man
void Player::walk(void) {

	// If the player and reloading 
	if (this->walkstate && !this->reloadstate) {


		// Play the footstep sounds at the beginning of the walk animation. 
		// We set the walk audio played to false to avoid playing it again, 
		// but we'll set the flag to true when the animation has been completed.
		// With this we do simulate a step sounds similar to reality
		if (!this->walkAudioPlayed && (this->tick==25 || this->tick == 50)) {

			// Get a random bool between 0,1 using the interval method described in the RNG class.
			// We do this to play sometimes the sx footstep, sometimes the right one.
			if (this->rng.getRngBool(0, 10)) this->engine->play2D("resources/audio/player/footstep_sx.mp3");
			else this->engine->play2D("resources/audio/player/footstep_dx.mp3");
			this->walkAudioPlayed = true;
		}
		
		// Update the animation
		if (this->inventory.weapons.at(this->currentweapon).walkz < 0.05f && !this->walkanim) {
			this->inventory.weapons.at(this->currentweapon).walkz += 0.001f;
		}
		else {
			this->walkAudioPlayed = false;
			this->walkanim = true;
		}

		if (this->inventory.weapons.at(this->currentweapon).walkz > 0 && this->walkanim) {
			this->inventory.weapons.at(this->currentweapon).walkz -= 0.001f;
		}
		else {
			this->walkanim = false;
		}

		// Every once half sec, play a walk sound

 

	} else {

		if (this->inventory.weapons.at(this->currentweapon).walkz > 0) {
			this->inventory.weapons.at(this->currentweapon).walkz -= 0.01f;
		}
		else {
			if (!this->shootstate && !this->aimstate && !this->reloadstate)
				this->idlestate = true;
		}

	}
	

}

// Function that changes the position of a weapon and perform a shoot, rotating the weapon 
// Simulating a shoot recoiil
void Player::shoot(void) {

	// To avoid continuous repetition of the shoot function, we impose that if the tick is the same 
	// (since the tick is updated 60 times in a single seconds) that means we're cycling on the same thick
	// due to the OpenGL idle func that throttle faster than the glUpdateFunc. With this we limit the update
	// to 60fps. 
	if (this->previoustick != this->tick) {

		// Play animation and Discard ammo from the weapon based on the current fire rate/ 
		// Basically, as we wrote above, we do a modulus operation in this way: imagining that the fire rate is 
		// 30 projectile per seconds, that means we must shoot every 0.02 msec. That because a full cycle of our 
		// loop is decided outside in the glutUpdateFunc, that does refresh our frame at 60FPS (so, in a second, 
		// we update the tick 60 times, hence tick=60 means that we reached a full second. 
		// An example could be: 
		// Tick = 0, and 60/firerate = 60/30 = 2. Now, doing 0 % 2 = 0, so we can perform a shoot animation and 
		// decrease an ammo.
		// Tick increases, now Tick = 1. 60/30 = 2 as before, and now doing 1%2 return 1. 
		// What that means? that means later in the if, doing !currate will return false, hence we do not shoot.
		// Again, doing Tick = 2, and 60/firerate = 60/30 = 2 means 2 % 2 = 0 and doing !0 = true later in the if.
		// See the pattern? We'll enter in the if 30 times in a total of 60, hence shooting every 0.02msec! 
		// Increasing or decreasing the fire rate will obviously makes the weapon slower/faster based on this principle.
		int currate = this->tick % int(round((60 / this->inventory.weapons.at(this->currentweapon).firerate)));

		// Store the current fire rate 
		this->currate = currate;


		// Choose the condition based on the weapon type or fire rate: That means basically that 
		// based on the type of the weapon (single or automatic) the condition to enter the shoot loop 
		// changes. 
		bool shootCondition = -1;
		TYPE curWeaponType = this->inventory.weapons.at(this->currentweapon).type;
		if (curWeaponType == TYPE::PISTOL || curWeaponType == TYPE::SHOTGUN) {

			// Basically if the weapon is a single shot type, we don't care about the current fire rate because
			// we can't use the automatic features of a rifle. So we just assign true to the shootCondition so 
			// we can enter the cycle.
			shootCondition = true;
		}
		else {

			// Instead if the weapon isn't single-shot, we need to check the current fire rate to understand if
			// we can perform a shoot or not.
			shootCondition = (bool)!currate;

		}

		// If the current tick matches the fire rate of the weapon or we are wielding a non-automatic weapon
		if (shootCondition) {

			// If we're shooting, not reloading and have suffficient ammo
			if (this->shootstate && !this->swapanim && !this->reloadstate && this->inventory.weapons.at(this->currentweapon).ammo > 0 && !this->inventory.weapons.at(this->currentweapon).singleShotWeaponFired) {

				///// ANIMATION
				// Reset animation leftovers 
				if (this->inventory.weapons.at(this->currentweapon).walkz != 0.0f) this->inventory.weapons.at(this->currentweapon).walkz = 0.0f;

				// Check for the shootz offset. If it is minor than 0.1 (recoil offset) and the 
				// recoil anim state isnt reached 
				if (this->inventory.weapons.at(this->currentweapon).shootz < 0.04f && !this->recoilanim) {
					this->inventory.weapons.at(this->currentweapon).shootz += 0.02f;

					// Update recoil: we update both centery to change the visual immediately, 
					// and then change recoily to sum up to the new position of the camera when the lookat 
					// function will be called on the mouse movement function.
					// NOTE THAT: recoil is a standard quantity since its defined by the shoot rate of the weapon. 
					// Faster weapon = higher recoil, slower weapon = lower recoil. 
					// If aiming, reduce recoil.
					float recoil = this->aimstate ? 0.0025 : 0.005;

					this->view.centery -= recoil * this->inventory.weapons.at(this->currentweapon).recoilMultiplier;
					this->view.recoily -= recoil * this->inventory.weapons.at(this->currentweapon).recoilMultiplier;


				}
				else {

					// Decrease ammo and set the anim to true 
					this->inventory.weapons.at(this->currentweapon).ammo--;
					this->recoilanim = true;

					// Play shoot sounds at the end of the animation 
					this->engine->play2D(this->inventory.weapons.at(this->currentweapon).shootSound.c_str(), false);

					// Set empty magazine to true if the current weapon has finished the ammo

					// Set shotFired to true if the weapon type is a single shot: doing so, we'll avoid
					// using it as an automatic waiting for the user releasing the button to set the shot fired to 
					// true again
					if (curWeaponType == TYPE::PISTOL || curWeaponType == TYPE::SHOTGUN) {
						this->inventory.weapons.at(this->currentweapon).singleShotWeaponFired = true;
						this->recoilanim = false;
						
					}
					

				}

				if (this->inventory.weapons.at(this->currentweapon).shootz > 0 && this->recoilanim) {
					this->inventory.weapons.at(this->currentweapon).shootz -= 0.02f;
				}
				else {
					this->recoilanim = false;
				}
				///// END ANIMATION		


			}

			// Else: something has fallen off, so we reset the shoot animation
			else {

			// If the weapon isn't a single shot type 
				if (this->inventory.weapons.at(this->currentweapon).shootz > 0) {
					this->inventory.weapons.at(this->currentweapon).shootz -= 0.02f;
				}
				else {
					if (!this->walkstate && !this->aimstate && !this->reloadstate)
						this->idlestate = true;
				}
				
			}

			// Play discharged weapon sound
			//if(this->emptyMagazine) this->engine->play2D("resources/audio/weapons/deploy.wav", false);

			// Update the previous tick with the current tick
			this->previoustick = this->tick;

		}

	}
	
}

// Function that changes the position of a weapon based on the reloadstate, rotating the weapon 
// Simulating a weapon reload
void Player::reload(void) {

	// Hangle reload animation 
	// If we have enough ammo 
	if (this->inventory.ammo > 0 && !(this->inventory.weapons.at(this->currentweapon).ammo == this->inventory.weapons.at(this->currentweapon).capacity)) {
		
		// Play the reload sounds and set the variable to true to 
		// avoid playing it again. Well set back the variable to false when the 
		// reload animation is finished
		if (!this->reloadAudioPlayed) {
			this->engine->play2D("resources/audio/weapons/reload.mp3");
			this->reloadAudioPlayed = true;
		}

		// If the reload animation isnt finished and we're not in idle state
		if (!this->reloadanim && !this->idlestate && !this->swapanim) {

			// Reset walk animation leftovers
			if (this->inventory.weapons.at(this->currentweapon).walkz != 0.0f) this->inventory.weapons.at(this->currentweapon).walkz = 0.0f;

			// Check for the reloady offset. If it is minor than a certain value (recoil offset) and the 
			// reload anim state isnt reached 
			if (this->inventory.weapons.at(this->currentweapon).reloadangle < 24.0f && !this->reloadanim) {
				this->inventory.weapons.at(this->currentweapon).reloadangle += 1.f;
			}
			else {
				this->reloadanim = true;
			}

		}

		// If the reload animation is finished or we back in idle state
		else {

			if (this->inventory.weapons.at(this->currentweapon).reloadangle >= 3.0f) {
				this->inventory.weapons.at(this->currentweapon).reloadangle -= 3.0f;
			}
			else {

				// If we not walking, aiming or shooting 
				if (!this->walkstate && !this->aimstate && !this->shootstate) {


					// If the reload state isnt completed 
					if (reloadstate) {
		
						// Get the ammo to replenish subtracting the capacity from the current ammo in the weapon 
						int toReload = this->inventory.weapons.at(this->currentweapon).capacity - this->inventory.weapons.at(this->currentweapon).ammo;

						// If there are enough ammo in the backpack to reload entirely our weapon 
						if (this->inventory.ammo > toReload) {

							// Reload our weapon entirely and subtract the amount from the backpack
							this->inventory.weapons.at(this->currentweapon).reload(this->inventory.weapons.at(this->currentweapon).capacity);
							this->inventory.ammo -= toReload;

						}
						else {

							// if not, replenish the ammo of the current weapon with what's left in the inventory and 
							// put the ammo in the backpack to 0
							this->inventory.weapons.at(this->currentweapon).reload(this->inventory.ammo + this->inventory.weapons.at(this->currentweapon).ammo);
							this->inventory.ammo = 0;

						}

					}

					// Reset the animations states
					this->reloadAudioPlayed = false;
					this->reloadstate = false;
					this->reloadanim = false;
					this->idlestate = true;

				}
			}
		}

	}
	else {

		// We don't have enough ammo to reload: just exit the function and restore the states
		this->reloadstate = false;
		this->reloadanim = false;
		this->idlestate = true;
	}

}


// Function that changes the position of a weapon based on the aimstate, rotating the weapon 
// Simulating an aim 
void Player::aim(void) {

	if (this->aimstate && !this->reloadstate && !this->swapanim) {

		// Play aim up sound once 
		if (!this->aimAudioPlayed) {
			this->engine->play2D("resources/audio/weapons/aimup.mp3");
			this->aimAudioPlayed = true;
		}

		// Reset walk and angle animation leftovers 
		if (this->inventory.weapons.at(this->currentweapon).walkz > 0.0f) this->inventory.weapons.at(this->currentweapon).walkz = 0.0f;
		if (this->inventory.weapons.at(this->currentweapon).reloadangle != 0.0f) this->inventory.weapons.at(this->currentweapon).reloadangle = 0.0f;
		
		// Increase the aim x and z coordinate
		if (this->inventory.weapons.at(this->currentweapon).aimx < 0.1f) this->inventory.weapons.at(this->currentweapon).aimx += 0.02f;
		if (this->inventory.weapons.at(this->currentweapon).aimz < 0.06f) this->inventory.weapons.at(this->currentweapon).aimz += 0.02f;

		// We use just one value to check if the maximum amount of rotation for the animation has been reached. 
		// If so, we set aimreached true
		if (this->inventory.weapons.at(this->currentweapon).aimangle < 10.0f) this->inventory.weapons.at(this->currentweapon).aimangle += 2.0f;
		else {
			this->aimtransition = false;
		}

	}
	else {

		// Play aim down sound once 
		if (this->aimAudioPlayed) {
			this->engine->play2D("resources/audio/weapons/aimdown.mp3");
			this->aimAudioPlayed = false;
		}

		// Reset the weapon to the original location
		if (this->inventory.weapons.at(this->currentweapon).aimx > 0.0f) this->inventory.weapons.at(this->currentweapon).aimx -= 0.02f;
		if (this->inventory.weapons.at(this->currentweapon).aimz > 0.0f) this->inventory.weapons.at(this->currentweapon).aimz -= 0.02f;
		if (this->inventory.weapons.at(this->currentweapon).aimangle > 0.0f) this->inventory.weapons.at(this->currentweapon).aimangle -= 2.0f;

		// Check for a single value to be 0: if so, we can start idle state (weapon finished the rest animation) 
		if (this->inventory.weapons.at(this->currentweapon).aimangle <= 0.0f && 
			this->inventory.weapons.at(this->currentweapon).aimx <= 0.0f && 
			this->inventory.weapons.at(this->currentweapon).aimz <= 0.0f) {
			
			// reset flags
			this->idlestate = true;
			this->aimtransition = false;

		}

	}
}

// Function that changes the position of a weapon based on the aimstate, rotating the weapon 
// Simulating a breathing
void Player::idle(void) {

	if (this->idlestate) {

		// Reset animation leftovers 
		if (this->inventory.weapons.at(this->currentweapon).walkz > 0.0f) this->inventory.weapons.at(this->currentweapon).walkz = 0.0f;
		if (this->inventory.weapons.at(this->currentweapon).reloadangle != 0.0f) this->inventory.weapons.at(this->currentweapon).reloadangle = 0.0f;
		
		// Reset raycaster
		this->raycaster.reset();
		
		///// IDLE ANIMATION
		if (this->inventory.weapons.at(this->currentweapon).aimz < 0.01f && !this->idleanim) {
			this->inventory.weapons.at(this->currentweapon).aimz += 0.00015f;
		}
		else {
			this->idleanim = true;
		}

		if (this->inventory.weapons.at(this->currentweapon).aimz > 0 && this->idleanim) {
			this->inventory.weapons.at(this->currentweapon).aimz -= 0.00020f;
		}
		else {
			this->idleanim = false;
		}

	}

}

// Function that takes care of swapping weapon of the player performing an animation.
void Player::swap(void) {

	// Assuring there are 2 weapon in the player weapon inventory 
	if (this->swapstate) {
		if (this->inventory.weapons.size() > 1) {

			// Swap audio play, we obviously play it once per swap. So we use an aid bool variable
			// to take track if the audio has been played or not.
			if (!this->swapAudioPlayed) {
				this->engine->play2D("resources/audio/weapons/swap.mp3");
				this->swapAudioPlayed = true;
			}

			if (!this->swapanim) {

				// Swapping weapon: performing an animation taking down the cur weapon
				// We basically move the weapon down the sight of the player by subtracting the aimy variables.
				if (this->inventory.weapons.at(this->currentweapon).aimy > -0.1f) {
					this->inventory.weapons.at(this->currentweapon).aimy -= 0.01f;
				}
				else {
					
					// Else, if the weapon aimy isn't lesser than 0.1 that means we currently swapped the weapon out
					// so we now change variables flags and set the new current weapon.
					this->swapanim = true;
					int newCurWeapon = this->currentweapon ? 0 : 1;
					this->currentweapon = newCurWeapon;

					// We also remember to set the aimy var to -.0.1 to simulate a back in animation
					this->inventory.weapons.at(this->currentweapon).aimy = -0.1f;

					// Reset the shot fired 
					this->inventory.weapons.at(this->currentweapon).singleShotWeaponFired = false;

				}

			}

			else {

				// Else we finished our swap anim and now apply the inverse transformation taking the weapon/
				// back up until is minor than the starting point.
				if (this->inventory.weapons.at(this->currentweapon).aimy < -0.01f) {
					this->inventory.weapons.at(this->currentweapon).aimy += 0.01f;
				}
				else {

					// Reset flags if the starring point matched
					this->swapAudioPlayed = false;
					this->swapanim = false;
					this->swapstate = false;
					this->idlestate = true;
				}

			}
		}
	}


}

// Function that given a weapon does assign it into the current weapon vector
void Player::assign(Weapon weapon) {

	// Handle case in which there are more than two weapon 
	this->inventory.weapons.push_back(weapon);

}

// Function that increase the internal tick counter to take track of time elapsed
void Player::updateTick(void) {

	// Based on the difficulty choosen, we update the tick accordingly
	if (this->tick > 60.0f) this->tick = 0;
	else this->tick++;

}

// Function that return the position vector of the player in the world
std::vector<float> Player::getCurrentPosition(void) {

	// Build the position vector
	std::vector<float> pos; 
	pos.push_back(this->view.eyex);
	pos.push_back(this->view.eyey);
	pos.push_back(this->view.eyez);
	
	// return the vector 
	return pos;

}

// Function that decreases player life and set the state of death if hp reach 0
void Player::takeDamage(float dmg) {

	if (this->hp > 0) {
		this->hp -= dmg;
		this->isHit = true;
	}
	else {
		this->hp <= 0;
		this->alive = false;
	}

	// Play sound 1/6 of the times being hit 
	if (!(this->tick % 10)) {

		if (this->rng.getRngBool(0, 10)) this->engine->play2D("resources/audio/player/hit_1.mp3", false);
		else  this->engine->play2D("resources/audio/player/hit_2.mp3", false);

	}

}

// Function that gather the ammo from the ground increasing the number into the inventory
void Player::lootAmmo(int quantity) {

	this->inventory.ammo += quantity;

	// Play ammo loot sound
	this->engine->play2D("resources/audio/effects/ammo.mp3", false);

}

// Function that gather the healthkit from the ground increasing the hp of the player
void Player::lootHealth(int quantity) {

	// if we need to heal
	if (this->hp < 100) {

		// Getting the missing hp from the player 
		int missingHp = 100 - this->hp;

		// If the quantity is major than the missing hp, just restore the hp to 100
		// else add the quantity to the hp
		if (quantity > missingHp) this->hp = 100;
		else this->hp += quantity;

		// Play health loot sound
		this->engine->play2D("resources/audio/effects/health.mp3", false);

	}

}

// Function that resets values when a new game starts 
void Player::reset(void) {

	// reset camera	
	this->view.reset();

	// Current player weapon is 0 initially 
	this->currentweapon = 0;

	// initial life 
	this->hp = 100;

	// Reset raycaster position 
	this->raycaster.reset();

	// Initial tick: this will serve as delimiter for animation et similia
	this->tick = 0;
	this->previoustick = -1;

	// Inventory 
	this->inventory.ammo = 250;

	// States: all initially false, except for idling and alive
	this->aimstate = false;
	this->aimtransition = false;
	this->shootstate = false;
	this->walkstate = false;
	this->reloadstate = false;
	this->swapstate = false;
	this->isHit = false;
	this->idlestate = true;
	this->alive = true;


	// Set the animation flag to false 
	this->reloadanim = false;
	this->recoilanim = false;
	this->idleanim = false;
	this->walkanim = false;
	this->swapanim = false;

	// Reset weapon position
	this->inventory.weapons.at(0).reset();
	this->inventory.weapons.at(0).reset();

	// Reload ammos
	this->inventory.weapons.at(0).reload(this->inventory.weapons.at(0).capacity);
	this->inventory.weapons.at(1).reload(this->inventory.weapons.at(0).capacity);

}

///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

// Function that init some of the values of the player
void Player::init(void) {

	// Init our view with a camera object
	this->view = Camera();

	// Current player weapon is 0 initially (or not? we can make the player spawn without weapon: TODO)
	this->currentweapon = 0;

	// initial life 
	this->hp = 100;

	// Reset raycaster position 
	this->raycaster.reset();

	// Initial tick: this will serve as delimiter for animation et similia
	this->tick = 0;
	this->previoustick = -1;

	// Inventory 
	this->inventory.ammo = 250;

	// States: all initially false, except for idling and alive
	this->aimstate = false;
	this->aimtransition = false;
	this->shootstate = false;
	this->walkstate = false;
	this->reloadstate = false;
	this->swapstate = false;
	this->isHit = false;
	this->idlestate = true;
	this->alive = true;

	
	// Set the animation flag to false 
	this->reloadanim = false;
	this->recoilanim = false;
	this->idleanim = false;
	this->walkanim = false;
	this->swapanim = false;

	// Audio booleans
	this->walkAudioPlayed = false;
	this->reloadAudioPlayed = false;
	this->hitAudioPlayed = false;
	this->swapAudioPlayed = false;
	this->aimAudioPlayed = false;
	this->emptyMagazine = false;

	// Set audio engine 
	this->engine = irrklang::createIrrKlangDevice();


}
