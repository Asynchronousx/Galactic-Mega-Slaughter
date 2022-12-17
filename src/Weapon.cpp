#include "Weapon.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////


// Function thata draw a weapon calling the inhereited render method.
void Weapon::draw(void) {

	// Wrapper for rendering an object
	this->render();

}

// Function that repleenish the ammo in the weapon 
void Weapon::reload(int amount) {

	// Charge back the ammo
	this->ammo = amount;

}

// Function that set a specific firerate
void Weapon::setFirerate(float firerate) {

	this->firerate = firerate;

}


// Function that set the damage of the weapon 
void Weapon::setDamage(int dmg) {

	this->dmg = dmg;

}

// Function that set the capacity of the weapon
void Weapon::setCapacity(int capacity) {

	this->capacity = capacity;

}

// Function that set the recoil of a weapon 
void Weapon::setRecoil(float recoil) {

	this->recoilMultiplier = recoil;

}

// Function that calls the superclass load asset
void Weapon::loadAsset(std::string path, std::string filename, std::string material_filename = "") {

	// Loading the model 
	this->load(Color4f(), path, filename, material_filename);

}

// Function that reset the object on a new game start
void Weapon::reset(void) {

	// Initial values of the weapon:
	// Aimx, aimy and aimz are the offset coordinate of our weapon for which, 
	// When the player press the right mouse button changes in the player class 
	// due to aim/rest animation. 
	this->aimx = 0.0f;
	this->aimy = 0.0f;
	this->aimz = 0.0f;

	// Shootz is the offset coordinate of our weapon for which when the player shoot, 
	// it will make the weapon recoil back in a short animation, values changes aswell
	// in the player class.
	this->shootz = 0.0f;

	/// Walkz is similar to shootz, but it's related to the walking animation
	this->walkz = 0.0f;

	// Reloady is used to take the weapon down  to simulate a reload (and so operate on the y)
	this->reloady = 0.0f;

	// Angle is the angle of rotation of the weapon when we aim 
	this->aimangle = this->reloadangle = 0.0f;


}


// Function that set the sound string for the desired weapon
void Weapon::setSound(std::string sound) {

	this->shootSound = sound;

}

// Setting weapon type
void Weapon::setType(TYPE type) {

	this->type = type;

}

///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

// Init function
void Weapon::init(void) {

	// Initial values of the weapon:
	// Aimx, aimy and aimz are the offset coordinate of our weapon for which, 
	// When the player press the right mouse button changes in the player class 
	// due to aim/rest animation. 
	this->aimx = 0.0f;
	this->aimy = 0.0f;
	this->aimz = 0.0f;

	// Ammo represent the current ammunition in the magazine 
	this->ammo = 24;

	// Dmg is the damage of the weapon
	this->dmg = 1;

	// Firerate represent the rate of fire per second of a given weapon.
	// I.e: if firerate is 35, then the weapon shoots 35 ammo per shot.
	// Default is 25 per second.
	this->firerate = 25;

	// Capacity magazine ammo of the weapon 
	this->capacity = 24;

	// Recoil for the weapon
	this->recoilMultiplier = 1;

	// Shootz is the offset coordinate of our weapon for which when the player shoot, 
	// it will make the weapon recoil back in a short animation, values changes aswell
	// in the player class.
	this->shootz = 0.0f;

	/// Walkz is similar to shootz, but it's related to the walking animation
	this->walkz = 0.0f;

	// Reloady is used to take the weapon down  to simulate a reload (and so operate on the y)
	this->reloady = 0.0f;

	// Angle is the angle of rotation of the weapon when we aim 
	this->aimangle = this->reloadangle = 0.0f;

	// Boolean that represent if a shot has been fired or not.
	// In the single non-automatic weapon, this variable is being set to true everytime a bullet has being
	// shot, to avoid shooting again while pressing the mouse. In the automatic weapon this variable
	// is ignored. 
	this->singleShotWeaponFired = false;

}

// Function that changes the position of a weapon and activate aimstate