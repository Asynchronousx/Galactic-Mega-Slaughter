#pragma once
#include "Object.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RNG.h"
#include "GameEnums.h"

#include "externals/IrrKlang/includes/irrKlang.h"

// Class enemy definition
class Enemy: public Object {

	public:

		// Attributes
		float posx, posy, posz;
		float dirangle;
		float scale;
		float speed;
		float hp;
		float power;
		bool alive;
		bool beingShot;
		irrklang::ISoundEngine* engine;
		IDENTIFIER identifier;
		std::vector<float> maxVertex;
		std::vector<float> minVertex;
		RNG rng;

		// Constructor 
		Enemy(IDENTIFIER identifier, GAMEMODE mode=GAMEMODE::NORMAL) {
			this->init(identifier, mode);
		}

		// Copy constructor
		//TODO: we need to copy the enemy instead of creating/loading multiple istances of an objects

		// Destructor 
		~Enemy() {}

		// Functions 
		void move(float, float, float);
		void draw(void);
		void loadAsset(Color4f, std::string, std::string, std::string);
		void randomize(GAMEMODE);
		int  getID(void);
		void takeDamage(int, int);
		void hoverSound();
		void hitSound();
		void deathSound();
		 
	private:

		// Attributes 

		// Functions 
		void init(IDENTIFIER, GAMEMODE);


};
		
