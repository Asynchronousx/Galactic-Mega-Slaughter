#define _CRT_SECURE_NO_WARNINGS
#include "externals/SOIL/includes/SOIL.h"
#include "TextureLoader.h"
#include <iostream>

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that will load and return a texture
GLuint TextureLoader::load(const char* filename, textype type)
{
	// We load a file (containing the texture)
	FILE* f = NULL;
	f = fopen(filename, "r");
	if (f == NULL)
	{
		fprintf(stderr, "cannot load file %s!", filename);
		exit(1);
	}

	// By the SOIL library we load a texture inside our variables.
	// We do specify some flags, as the LOAD_AUTO, CREATE NEW ID that create a new id for the texture and 
	// the use the minmap flag, that specify that at long distances we must use SMALLER resolution
	// of our texture to avoid useless computational costs.
	GLuint t = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	// We then bound the texture to the variable
	glBindTexture(GL_TEXTURE_2D, t);

	// We'll use magnification and minification for our texture using the neares neighbour, 
	// wrapping texture, cube map and clamp to edge for the sky and repeat for the ground texture.
	if (type == GROUND) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	else if (type == SKY) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	// returning the created texture
	return t;

}