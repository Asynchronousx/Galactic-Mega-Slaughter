#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "GameEnums.h"

// Enum for directions 


// Class camera definition
class Camera {

	public:
		// Attributes
		float eyex, eyey, eyez;
		float centerx, centery, centerz;
		float sensitivity;
		float recoily;
		int width;
		int height;

		// Functions
		Camera();
		Camera(int, int, float, float, float, float, float, float, float, float);
		~Camera() {}
		float compute_centers(AXIS);
		void compute_directions(DIRECTION);
		void update_centers(int, int);
		void update_window(float, float);
		void update(void);
		void restore_speed(void);
		void decrease_speed(void);
		void reset(void);

	private:

		// Attributes
		float speed;

		// Functions
		float to_radians(float);
		void  view_constraint(float);

};