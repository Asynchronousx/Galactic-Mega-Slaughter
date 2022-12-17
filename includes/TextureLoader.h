#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GameEnums.h"

// Class that will take care of the loading of various texture.
class TextureLoader {

	public:
		GLuint load(const char*, textype);
		//bool release(const std::string);
		
};