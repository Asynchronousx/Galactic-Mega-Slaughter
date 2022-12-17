//  Camera class function implementation
#define TO_RADIANS 3.141592 / 180.0
#include "Camera.h"
#include <iostream>

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// default constructor
Camera::Camera()
{

	this->width = 0;
	this->height = 0;
	this->eyex = 0.0f;
	this->eyey = 2.0f;
	this->eyez = 5.0f;
	this->recoily = 0.0;
	this->centerx = 0.0f;
	this->centery = 1.0f;
	this->centerz = -1.0f;
	this->speed = 0.8f;
	this->sensitivity = 0.01f;

}

// constructor that takes arguments 
Camera::Camera(int w, int h, float ex, float ey, float ez, float cx, float cy, float cz, float sp, float se)
{

	this->width = w;
	this->height = h;
	this->eyex = ex;
	this->eyey = ey;
	this->eyez = ez;
	this->centerx = cx;
	this->centery = cy;
	this->centerz = cz;
	this->speed = sp;
	this->sensitivity = se;

}


// Function that will return the summation of the eye of the camera and the updated center
// in the second row of the glulookat func
float Camera::compute_centers(AXIS axis) {

	switch (axis) {

	case AXIS::X:
		return this->eyex + this->centerx;

	case AXIS::Y:
		return this->eyey - this->centery;

	case AXIS::Z:
		return this->eyez + this->centerz;

	}

}

// Function that will update the lookat values based on the mouse position
void Camera::update_centers(int x, int y)
{

	// Warp the pointer to the new y location based on recoil (if present)
	// We basically solve a linear equation multiplying both side for the variable 
	// without the x to solve. I.e: recoily is 0.005 = (y - 250)*0.1. We divide both for 
	// 0.01 and we obtain 0.5 = y - 250. From here it is easy to solve.
	float yy = y;
	float xx = x;
	yy += this->recoily / this->sensitivity;
	this->recoily = 0.;

	// If we rotate the camera around on the x-axis, we want to warp the pointer back to the 
	// maximum widht or 0 if a limit is reached, so we can rotate the camera infinitely. 
	std::cout << "width - 2: " << this->width - 2 << " x: " << x << std::endl;
	if (xx >= this->width-2) xx = 0;
	else if (xx <= 0) xx = this->width;
	std::cout << "new xx : " << xx << std::endl;
	// At last, we warp the point to our new locations: only if the values has been changed.
	// This is because without this control the animation do have a strange behaviour
	if (xx != x || yy != y) glutWarpPointer(xx, yy);


	// Get mouse position scaling by the center of the scene
	float x_pos = (xx - (this->width / 2)) * this->sensitivity;
	float y_pos = (yy - (this->height / 2)) * this->sensitivity;


	// Get the angle of the coordinate: we just use sin for both x/y and 
	// - cos for the z coordinate (since we're looking into the scene)
	this->centerx = sin(x_pos);
	this->centery = sin(y_pos);
	this->centerz = -cos(x_pos);
	
	// Then limit the vertical view angle 
	this->view_constraint(y_pos);

}

// Function that will update the lookat values based on keyboard key pressed
void Camera::compute_directions(DIRECTION direction) {

	// Given the direction we update the corrispondent value
	// We're moving left/right/up/down with the keyboard. For example, to achieve a 360 degree rotation
	// based on the camera angle given by the mouse, we add to the x variable the z angle and 
	// the z the x one in the A (left) case. Same happens with D. 
	// With W/S, since we just want too move forward and backward the camera, we add the correct 
	// angle to the x and z variable.
	switch (direction) {

	case DIRECTION::FORWARD:
		this->eyex += this->centerx * this->speed;
		this->eyez += this->centerz * this->speed;
		break;

	case DIRECTION::BACKWARD:
		this->eyex -= this->centerx * this->speed;
		this->eyez -= this->centerz * this->speed;
		break;

	case DIRECTION::LEFT:
		this->eyex += this->centerz * this->speed/1.5;
		this->eyez -= this->centerx * this->speed/1.5;
		break;

	case DIRECTION::RIGHT:
		this->eyex -= this->centerz * this->speed/1.5;
		this->eyez += this->centerx * this->speed/1.5;
		break;

	}

}


// Function that will update the windows h/w values
void Camera::update_window(float w, float h) {

	// Updating w and h values 
	this->width = w;
	this->height = h;

}

// Function that update the new position with the values calculated earlier in other functions
void Camera::update(void) {

	// Looking at the coordinate calculated by moving mouse/keyboard. 
	// The first three parameters: the camera position vector. 
	// The second three parameters: the point we're looking at (current location + displacement)
	// Thee last three parameters: the UP vector 
	gluLookAt(
		this->eyex, this->eyey, this->eyez,
		this->compute_centers(AXIS::X), this->compute_centers(AXIS::Y), this->compute_centers(AXIS::Z),
		0.0f, this->eyey, 0.0f
	);

}


// Function that restores the speed of the camera
void Camera::restore_speed(void) {

	this->speed = 0.8;

}

void Camera::decrease_speed(void) {
	
	this->speed = 0.1;

}

// Function that reset the values when a new game starts
void Camera::reset(void) {

	glutWarpPointer(this->width / 2, this->height / 2);
	this->eyex = 0.0f;
	this->eyey = 2.0f;
	this->eyez = 5.0f;
	this->recoily = 0.0;
	this->centerx = 0.0f;
	this->centery = 1.0f;
	this->centerz = -1.0f;
	this->speed = 0.8f;
	this->sensitivity = 0.01f;

}

///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

// Function that update the vertical view constraing by a given angle
void Camera::view_constraint(float y_pos) {

	// Simple vertical view angle constraint, if exceed 45 degree then set it
	// back to 45 (both for up/down vertical view).
	// Furthermore, since the sin is periodic, we just need check also if the y_pos 
	// variable exceed a fixed amount: in our code, if the y_pos var is major than 1.57
	// then we start to rotate the other way around (doing a 360deg rotation). 
	// To avoid that, we also include the y_pos in the constraint check.
	if (abs(y_pos) >= 1.57) {
		int sgn = y_pos > 0 ? 1 : -1;
		this->centery = 1.0f*sgn;
	}

}

// Function that converts angle to radians
float Camera::to_radians(float angle) {

	// Returning the radians given an angle
	return angle * TO_RADIANS;

}