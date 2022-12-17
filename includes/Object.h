#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "externals/OBJLoader/Model.h"
#include "Utils.h"
#define OBJ_LIBRARY
using namespace objloader;

#include <iostream>

// Header that defines a simple wrapper class for the .OBJ/.MTL file loader class.
// This wrapper will embed a model inside the defined class, making easy to load/render 
// given OBJ/MTL files.
class Object {

public:
	
	// Attributes
	Model model;

	// Constructor
	Object() {};
	Object(std::string path, std::string objfile, std::string mtlfile = "") {
		this->path = path;
		this->objfile = objfile;
		this->mtlfile = mtlfile;
	}

	// Destructor
	~Object() {};

	// Functions that load an OBJ file (MTL if specified) into the model structure. 
	// Overloaded for different calals
	void load() {
		this->model.load(this->path, this->objfile, this->mtlfile);
	}

	// Function that load an OBJ file (MTL if specified) into the model structure
	void load(Color4f colors, std::string path, std::string filename, std::string material_filename = "") {

		if (material_filename.empty()) this->model.load(path, filename);
		else this->model.load(path, filename, material_filename, colors);

	}

	// Function that render the current object (if present)
	void render() {
		this->model.render();
	}

private:
	std::string path;
	std::string objfile;
	std::string mtlfile;

}; 
