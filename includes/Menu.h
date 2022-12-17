#pragma once
#include "TextureLoader.h"

// Simple class container wrapping various textures used as a menu interface
class Menu {

	public:

		// Attributes
		GLuint title;
		GLuint difficulty;
		GLuint loading;
		GLuint scoreboard;
		GLuint about;
		GLuint gameover;

		// Constructor/Destructor
		Menu() {}
		~Menu() {}

};