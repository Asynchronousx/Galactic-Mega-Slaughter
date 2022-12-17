#include "ConsumablesManager.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that will be called when a new consumable needs to be spawned 
void ConsumablesManager::spawnHealthbox(Vector3f position, float scale, float angle, int multiplier) {

	Healthbox newbox;
	newbox.setConsumableInfo(position, scale, angle);
	newbox.setID(++this->curHealthID);
	newbox.amount *= multiplier;
	this->healthboxes.push_back(newbox);

}

// Function that will be called when a new consumable needs to be spawned 
void ConsumablesManager::spawnAmmobox(Vector3f position, float scale, float angle, int multiplier) {

	Ammobox newbox;
	newbox.setConsumableInfo(position, scale, angle);
	newbox.setID(++this->curAmmoID);
	newbox.amount *= multiplier;
	this->ammoboxes.push_back(newbox);

}

// Function that loads object template into it: 0 reference healthbox, 1 ammoboxes
void ConsumablesManager::assignTemplates(void) {

	// if we haven't loaded the templates yet (i.e: first game)
	if (!this->templateLoaded) {

		// Declaring two enemies, hit and normal and assigning a useless id (-1)
		Consumable* healthbox = new Consumable();
		Consumable* ammobox = new Consumable();

		// Load their assets with respective colors
		ammobox->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/consumables/ammobox/", "ammobox.obj", "ammobox.mtl");
		healthbox->loadAsset(Color4f(1.0, 1.0, 1.0, 1.0), "resources/models/consumables/aidkit/", "aidkit.obj", "aidkit.mtl");

		// Assign the enemies to the vector
		this->Templates.emplace_back(healthbox);
		this->Templates.emplace_back(ammobox);

		// flag true tmp loaded
		this->templateLoaded = true;

	}

}

// Function that given an id will perform an elimination from the relative vector of the 
// consumables 
void ConsumablesManager::deleteHealthbox(int id) {

	// Since the vector may be subject to various deleting through time, the ID of the consumable
	// won't necessarily correspond to its index. We then iterate through the consumable vector 
	// to find the index in which the looted box needs to be deleted
	int realID = -1;
	for (int i = 0; i < this->healthboxes.size(); i++) {

		// If the id correspond to the one passed in input, assign the real id
		// to be deleted 
		if (this->healthboxes.at(i).id == id) realID = i;

	}

	// Delete the box from the vector at the given id
	this->healthboxes.erase(this->healthboxes.begin() + realID);

	// Decrease the curID
	this->curHealthID--;

}


// Function that given an id will perform an elimination from the relative vector of the 
// consumables 
void ConsumablesManager::deleteAmmobox(int id) {

	// same reasons as above
	int realID;
	for (int i = 0; i < this->ammoboxes.size(); i++) {

		// If the id correspond to the one passed in input, assign the real id
		// to be deleted 
		if (this->ammoboxes.at(i).id == id) realID = i;

	}

	// Delete the box from the vector from the given id
	this->ammoboxes.erase(this->ammoboxes.begin() + realID);

	// Decrease the curID
	this->curAmmoID--;

}

// Function that reset the values when a new game starts
void ConsumablesManager::reset(void) {

	this->healthboxes.clear();
	this->ammoboxes.clear();
	this->curAmmoID = -1;
	this->curHealthID = -1;

}