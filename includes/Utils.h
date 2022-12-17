#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

// Class that contains some of the utils function needed in various computations
class Utils {

	public:
		
		// Constructor/Destructor 
		Utils() {}
		~Utils() {}

		// Functions 
		bool inRange(float, float, float);
		bool areFloatsEqual(float, float);
		

};

// Class that represent a color 
class Color4f {

	public:

		// Attributes
		float red;
		float blue;
		float green;
		float alpha;

		// Constructor/Destructor
		Color4f(float red = 1.0, float green = 1.0, float blue = 1.0, float alpha = 1.0) {

			this->red = red;
			this->blue = blue;
			this->green = green;
			this->alpha = alpha;

		}

		~Color4f() {}

};


// Simple vector class that represent a triplete x,y,z
class Vector3f {

	public:

		// Attributes
		float x;
		float y;
		float z;

		// Constructor/Destructor
		Vector3f(float x, float y, float z) {
			this->x = x;
			this->y = y;
			this->z = z;			
		}

		~Vector3f() {}


};

// Class that does the raycasting computations
class Raycaster {

public:

	// Attributes
	GLdouble x;
	GLdouble y;
	GLdouble z;

	// Constructor/Destructor 
	Raycaster() {}
	~Raycaster() {}

	// Methods 
	void project(int, int);
	void reset(void);

};

// Font class that handles various draw cases
class Font {

	public:
		
		// Constructor/Destructor 
		Font() {}
		~Font() {}

		// Functions
		void drawStrokeFont(Vector3f, void*, char*, Color4f, float);
		void drawStrokeFontInterface(Vector3f, Vector3f, float, void*, char*, Color4f, float);
		void drawBitmapFont(void*, char*, Vector3f);


};

// Class that does manage the string format for the nameplates 
class GameString {

	public:

		// Attributes
		int minSize;
		int maxSize;
		std::string data;

		// Constructor/Destructor
		GameString() {
			this->minSize = 3;
			this->maxSize = 8;
			this->data = "";		
		}

		~GameString() {}

		// Functions
		void addStringKey(unsigned char);
		void deleteStringKey(void);
		void setMinSize(int);
		void setMaxSize(int);
		bool reachedMinSize(void);

};