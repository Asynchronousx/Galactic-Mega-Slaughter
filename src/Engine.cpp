#include "Engine.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////
// 
//  Function that setup the init routines and object for the engine class
void Engine::setup(void) {

	// Creating a new player 
	this->player = Player();

	// Initializing a drawer
	this->drawer = Drawer(0);

	// Init object loading 
	this->environment.load(Color4f(), "resources/models/planets/", "flandscape.obj", "flandscape.mtl");
	//barrel.load("data/barrel/", "barrel.obj", "barrel.mtl");
	//truck2.load("data/barrel/", "truck.obj", "truck.mtl");

	// Loading a weapon
	Weapon m4a1, ak47, ak, m4;
	m4a1.load(Color4f(), "resources/models/weapons/m16/", "m16.obj", "m16.mtl");
	ak47.load(Color4f(), "resources/models/weapons/ak47/", "ak47.obj", "ak47.mtl");

	// Setting weapons properties
	ak47.setFirerate(20);
	m4a1.setFirerate(45);
	m4a1.setCapacity(32);
	ak47.setCapacity(42);
	ak47.reload(42);
	m4a1.reload(32);
	m4a1.setDamage(7);
	ak47.setDamage(12);
	m4a1.setRecoil(1);
	ak47.setRecoil(1.3);

	// Assign those weapons to the player
	this->player.assign(m4a1);
	this->player.assign(ak47);

	// PLAYER REFERENCE ASSIGN! FIX IT TO FIT INTO THE CONSTRUCTOR.
	// I know its a bad practice assigning the reference to something else but i need 
	// things done fast lmao 
	this->drawer.player = &player;
	this->drawer.enemyManager = &this->enemyManager;
	this->drawer.consumablesManager = &this->consumableManager;
	this->spawnHandler.consumableManager = &this->consumableManager;
	this->spawnHandler.enemyManager = &enemyManager;
	this->collisionHandler.player = &this->player;
	this->collisionHandler.handler = &this->spawnHandler;

	// WTF??
	ak.load(Color4f(), "resources/models/weapons/ak47/", "ak47.obj", "ak47.mtl");
	m4.load(Color4f(), "resources/models/weapons/m16/", "m16.obj", "m16.mtl");

	// Creating enemy and consumables templates loading their relative models
	this->enemyManager.assignTemplates();
	this->consumableManager.assignTemplates();

}

// Function that updates the tick of the various istances
void Engine::update(void) {

	// updating player tick: resetting if equal to 60, increasing if not
	this->player.updateTick();

	// Aswell updating the enemy manger tick, resetting it at 240;
	this->enemyManager.updateTick();

	// Updating the drawer tick, will draw how much time has passed since the start
	this->drawer.updateTick();

}

// Function that render the scene based on the current game state 
void Engine::render(void) {

	// Clearing procedures 
	this->drawer.clear();

	// Updating the camera 
	this->player.updateView();

	// Drawing the scene
	this->drawer.drawScene(this->environment);

	// Check for collisions 
	this->collisionHandler.checkCollisions();

	// Managing enemy routines (moving, check death)
	this->enemyManager.manageEnemies(player.getCurrentPosition());

	// Draw the rest of the scene (needs to be on front on everything)
	this->drawer.drawUtil();

	// We then call the spawn function to check if a new enemy must be spawn
	this->spawnHandler.spawnEnemy();

	// Swap buffer for drawing
	glutSwapBuffers();

}

// Function that resize the window with the desired w,h
void Engine::resize(int w, int h) {

	// Setting ratio and new window size
	float ratio = w * 1.0 / h;

	// Updating camera values 
	this->player.view.update_window(w, h);

	// Setting the viewport
	glViewport(0, 0, w, h);

	// Setting the projection 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(65.0f, ratio, 0.1, 450.0);

	// Resettign the modelview matrix 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Redisplay the scene
	glutPostRedisplay();

}

// Function that handles the mouse moving when pressing a button
void Engine::activeLook(int x, int y) {

	// To avoid freezing the scene we need to redisplay:
	// We call the raycast function that will check if the player is shooting, so if we move our
	// aim during the shoot phase, the sight will follow
	player.raycast();
	glutPostRedisplay();

}

// Function that handles the mouse passive moving 
void Engine::passiveLook(int x, int y) {

	// Raycast if moving the mouse to upgrade position (if needed)
	this->player.raycast();

	// Computing the new view centers
	this->player.look(x, y);

	// redisplay the scene
	glutPostRedisplay();

}

// Function that handles the mouse button pressing when aiming
IDLEFUNC Engine::aim(int buttonState) {

	// If we're pressing it
	if (buttonState == GLUT_DOWN) {

		// If the player isn't reloading (can't aim while reloading :p)
		if (!this->player.reloadstate) {

			// Disable idle and activate aimstate
			this->player.aimtransition = true;
			this->player.idlestate = false;
			this->player.aimstate = true;

			// Set the idle func 
			return IDLEFUNC::AIM;

		}

	}

	// If we released it
	else if (buttonState == GLUT_UP) {

		// If the player is not reloading (can't change aim status while reloading :p)
		if (!this->player.reloadstate) {

			// Disable aimstate
			this->player.aimtransition = true;
			this->player.aimstate = false;

			// Set the aim func again to reset the aim animation
			return IDLEFUNC::AIM;

		}

	}


}

// Function that handles the mouse button pressing when shooting
IDLEFUNC Engine::shoot(int buttonState) {

	// If the button is being pressed
	if (buttonState == GLUT_DOWN) {

		// If the player isn't reloading (can't shoot while reloading :p)
		// and isnt transitioning into the aim position
		if (!this->player.reloadstate && !this->player.aimtransition) {

			// Idle is false, shootstate is true
			this->player.idlestate = false;
			this->player.shootstate = true;

			// Raycast projection of the mouse click
			this->player.raycast();

			// Set the idle func to perform the animation 
			return IDLEFUNC::SHOOT;

		}

	}

	// BUTTON UP
	else if (buttonState == GLUT_UP) {

		// Stop shooting: we do not call the shoot function again because when we stop shooting 
		// we need to be able to still aim walk etc
		this->player.shootstate = false;

		// Reset the projection 
		this->player.raycaster.reset();

		if (this->player.aimstate || this->player.aimtransition) return IDLEFUNC::AIM;
		else this->player.idlestate = true;

	}


}

// Function that handles the mouse button pressing when swapping weapon
IDLEFUNC Engine::swapWeapon(void) {

	// Assuring we're not aiming, shooting or reloading
	if (!this->player.shootstate && !this->player.aimstate && !this->player.reloadstate) {

		this->player.idlestate = false;
		this->player.swapstate = true;
		return IDLEFUNC::SWAP;

	}

}

// Function that handles the keyboard button pressing 
IDLEFUNC Engine::performActionOnPress(unsigned char key) {

	// Case switch with keyboard directions
	switch (key) {

	case 'W':
	case 'w':

		// If the player isn't:
		// Shooting, aiming and reloading
		// AND if the player is in idle state: 
		if (!this->player.shootstate && !this->player.aimstate && !this->player.reloadstate && this->player.idlestate) {

			// Disable idlestate and enable walking state
			this->player.idlestate = false;
			this->player.walkstate = true;

		}

		// Regardless of the animation or not we move in the desired direction 
		// (this to, i.e: moving while shooting etc).
		this->player.changePosition(DIRECTION::FORWARD);

		// If we have activated the walk state, we need to perform an animation 
		return IDLEFUNC::WALK;

		break;

	case 'S':
	case 's':

		// SAME AS ABOVE
		if (!this->player.shootstate && !this->player.aimstate && !this->player.reloadstate && this->player.idlestate) {
			this->player.idlestate = false;
			this->player.walkstate = true;
		}

		// Regardless of the animation or not we move in the desired direction 
		// (this to, i.e: moving while shooting etc).
		this->player.changePosition(DIRECTION::BACKWARD);
		return IDLEFUNC::WALK;
		
		break;

	case 'A':
	case 'a':

		// When moving left or right we dont want to play no animation, so we just change
		// the direction
		this->player.changePosition(DIRECTION::LEFT);
		break;

	case 'D':
	case 'd':

		// SAME AS ABOVE
		this->player.changePosition(DIRECTION::RIGHT);
		break;

	case 'R':
	case 'r':

		// If the player isn't: shooting, aiming, walking and if its idle
		if (!this->player.shootstate && !this->player.aimstate && !this->player.walkstate && this->player.idlestate) {

			// idlestate is false and we activate the reloadstate
			this->player.idlestate = false;
			this->player.reloadstate = true;

			// Set the current idle func
			return IDLEFUNC::RELOAD;
		}
		break;

	case 'T':
	case 't':
		this->drawer.fogdensity -= 0.01;
		break;

	case 'Y':
	case 'y':
		this->drawer.fogdensity += 0.01;
		break;;
	}


}

// Function that handles the mouse button releasing
IDLEFUNC Engine::performActionOnRelease(unsigned char key) {

	// Case switch with keyboard directions: this action 
	// will be performed when the key is switched up, so we just revert
	// the states
	switch (key) {

	case 'W':
	case 'w':
		this->player.walkstate = false;
		this->player.idlestate = true;
		return IDLEFUNC::WALK;
		break;
	case 'S':
	case 's':
		player.walkstate = false;
		player.idlestate = true;
		return IDLEFUNC::WALK;
		break;

	case 'A':
	case 'a':
		player.walkstate = false;
		return IDLEFUNC::WALK;
		break;

	case 'D':
	case 'd':
		player.walkstate = false;
		return IDLEFUNC::WALK;
		break;

	}

}


///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////
