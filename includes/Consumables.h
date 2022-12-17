#pragma once
#include "Utils.h"
#include "Object.h"

// Class represent the items a player can "consume" to gain some benefits. 
class Consumable : public Object {

	public:

		// Attributes 
		int id;
		float x;
		float y;
		float z;
		float scale;
		float angle;
		float height;
		bool floating;

		// Constructor/Destructor 
		Consumable() {
			

			// Initially, the cosumable floating state is true representing the 
			// need for a float animation inside the drawer function 
			this->floating = true;

		}
		~Consumable() {}

		// Functions 
		void draw(void);
		void setConsumableInfo(Vector3f, float, float);
		void loadAsset(Color4f, std::string, std::string, std::string);
		void setID(int);


};


// Class Ammobox and healtbox: thus i could avoid to define those, due to some project engineering 
// define independent classes to follow the SOLID principles is the right thing do to. 
// Done that subclass extension, we could easily write individual property onto it.
class Ammobox : public Consumable {

	public:

		// Attributes 
		int amount; 

		// Constructor/Destructor 
		Ammobox() {

			// Amount represent the amount of ammo restored that will be multiplied
			// by a scale factor
			this->amount = 15;

		}

		~Ammobox() {}


};

class Healthbox : public Consumable {

	public:

		// Attributes 
		int amount;

		// Constructor/Destructor 
		Healthbox() {

			// Amount represent the amount of health restored that will be multiplied
			// by a scale factor
			this->amount = 5;

		}
		~Healthbox() {}


};