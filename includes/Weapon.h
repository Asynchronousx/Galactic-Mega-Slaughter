#pragma once
#include "Object.h"
#include "GameEnums.h"

// Weapon class that takes cares of the weapon behaviour and info. We do extends 
// The object class since the weapon is a 3D model derived from an obj/mtl file.
class Weapon: public Object {

	public:

		// Attributes;
		float aimx, aimy, aimz;
		float shootz;
		float walkz;
		float reloady;
		float aimangle, reloadangle;
		float firerate;
		float recoilMultiplier;
		int ammo;
		int capacity;
		int dmg;
		TYPE type;
		bool singleShotWeaponFired;
		std::string shootSound;
		

		// Constructor
		Weapon() {
			// Init values 
			this->init();
		}


		// Destructor 
		~Weapon() {}

		// Functions
		void reload(int);
		void setFirerate(float);
		void setDamage(int);
		void setCapacity(int);
		void setRecoil(float);
		void setType(TYPE);
		void setSound(std::string);
		void draw(void);
		void loadAsset(std::string, std::string, std::string);
		void reset(void);

	private:

		// Attributes
		
		// Functions 
		void init(void);

};