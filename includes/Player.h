#pragma once
#include "Inventory.h"
#include "Camera.h"
#include "Utils.h"
#include "RNG.h"
#include "externals/IrrKlang/includes/irrKlang.h"

// Class that represent a player entity inside our world. Our player does multiple things 
// and interaction with the external scene. 
class Player {

	public: 
		// Attributes

		Camera view;
		Raycaster raycaster;
		Inventory inventory;
		GameString name;
		RNG rng;
		irrklang::ISoundEngine* engine;
		float hp;
		int currentweapon;
		int tick;
		int previoustick;
		int currate;
		
		bool aimstate;
		bool aimtransition;
		bool shootstate;
		bool idlestate;
		bool walkstate;
		bool reloadstate;
		bool swapstate;
		bool isHit;
		bool alive;
		bool emptyMagazine;

		// Constructor 
		Player() {
			this->init();
		}

		// Destructor 
		~Player() {}

		// Functions 
		
		void updateView(void);
		void checkDeath(void);
		void changePosition(DIRECTION);
		void assign(Weapon weapon);
		void walk(void);
		void look(int, int);
		void raycast(void);
		void reload(void);
		void aim(void);
		void rest(void);
		void shoot(void);
		void idle(void);
		void swap(void);
		void updateTick(void);
		void takeDamage(float);
		void lootAmmo(int);
		void lootHealth(int);
		void reset(void);
		std::vector<float> getCurrentPosition(void);


	private:

		// Attributes 
		bool recoilanim;
		bool reloadanim;
		bool idleanim;
		bool walkanim;
		bool swapanim;
		bool walkAudioPlayed;
		bool reloadAudioPlayed;
		bool hitAudioPlayed;
		bool swapAudioPlayed;
		bool aimAudioPlayed;


		// Functions 
		void init(void);

};
