#include "Utils.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that given a value, a centroid and his offset will return true/false
// based on the fact that the values is containe or not in that interval
bool Utils::inRange(float val, float centroid, float offset)
{
    
    // Basically we use a centroid value to calculate an high/low value range in which 
    // with the given offset we check if val is contained in (low, val, high). 
    // We calculate the extreme of the interval
    float high = centroid + offset;
    float low = centroid - offset;

    // Then we check if the value is or not in that given interval, returning a bool
    return (low <= val && val <= high);

}

// Function that returns the equality between two float using the epsilon machina
bool Utils::areFloatsEqual(float x, float y) {

		return fabs(x - y) <= std::numeric_limits<float>::epsilon();

}

// Function that does perform a raycast based on the clicked projection onto the screen with the mouse
void Raycaster::project(int x, int y) {

	
	// Declaring object we need to apply the unprojection
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	//GLfloat winX, winY;
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLfloat winX = 0.0, winY = 0.0, winZ = 0.0;
	GLdouble posX = 0.0, posY = 0.0, posZ = 0.0;
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;   // invert winY so that down lowers value
	glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	this->x = posX;
	this->y = posY;
	this->z = posZ;

	/*
	// obtain the Z position (not world coordinates but in range 0 - 1)
	GLfloat z_cursor;
	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z_cursor);
	
	// obtain the world coordinates
	gluUnProject(winX, winY, z_cursor, modelview, projection, viewport, &this->x, &this->y, &this->z);
	*/

}

// Function that reset the current casting on the variables
void Raycaster::reset(void) {

	this->x = 0.;
	this->y = 0.;
	this->z = 0.;

}

// Function that renders a stroke character bitmap given a scale, position.
void Font::drawStrokeFont(Vector3f position, void* font, char* string, Color4f colors, float scale) {

	// TMP char
	char* c;

	// Push a new matrix onto the stack to avoid modifying the current 
	glPushMatrix();

	// Translate,rotate,scale
	glTranslatef(position.x, position.y, position.z);
	glScalef(scale, scale, scale);

	glColor3f(colors.red, colors.green, colors.blue);

	// Draw the string
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}

	// Restore 
	glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();

}

void Font::drawStrokeFontInterface(Vector3f position, Vector3f rotation, float angle, void* font, char* string, Color4f colors, float scale) {

	// TMP char
	char* c;

	// Push a new matrix onto the stack to avoid modifying the current 
	glPushMatrix();

	// Load identity to stick the font to the interfce
	glLoadIdentity();

	// Translate,rotate,scale
	glTranslatef(position.x, position.y, position.z);
	glScalef(scale, scale, scale);
	glRotatef(angle, rotation.x, rotation.y, rotation.z);
	glColor3f(colors.red, colors.green, colors.blue);

	// Draw the string
	for (c = string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}

	// Restore 
	glColor3f(1.0, 1.0, 1.0);
	glPopMatrix();

}

// Function to draw a bitmap character string.
void Font::drawBitmapFont(void* font, char* string, Vector3f position) {

	// Tmp char
	char* c;

	// Setting the position 
	glRasterPos3f(position.x, position.y, position.z);

	// Drawing the string
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);

}


// Function that adds a char key to the string data if possible
void GameString::addStringKey(unsigned char key) {

	// If the max size isnt reached	
	if (this->data.size() < this->maxSize) {
		this->data += key;
	}

}

// Function that delete a key into the given data if present
void GameString::deleteStringKey(void) {

	// if some data 
	if (this->data.size() > 0) {
		this->data.pop_back();
	}

}

// Set min/max string size
void GameString::setMinSize(int size) {

	this->minSize = size;


}

// Set min/max string size
void GameString::setMaxSize(int size) {

	this->maxSize = size;

}

// Function that return a bool representing if min size has been reached or not
bool GameString::reachedMinSize(void) {

	return this->data.size() >= this->minSize ? true : false;

}