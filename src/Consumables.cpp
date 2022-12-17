#include "Consumables.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function thata draw a weapon calling the inhereited render method.
void Consumable::draw(void) {

	// Wrapper for rendering an object
	this->render();

}

// Function that assing given position in the world to a consumable
void Consumable::setConsumableInfo(Vector3f position, float scale, float angle) {

	// Assign the info about position,scale,angle
	this->x = position.x;
	this->y = position.y;
	this->z = position.z;
	this->scale = scale;
	this->angle = angle;

}

// Function that calls the parent load function to load an object file
void Consumable::loadAsset(Color4f colors, std::string path, std::string filename, std::string material_filename = "") {

	// Loading the model 
	this->load(colors, path, filename, material_filename);

}

// Function that set the id of the consumable 
void Consumable::setID(int id) {

	this->id = id;

}
