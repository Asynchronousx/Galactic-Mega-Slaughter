#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include "Drawer.h"
#include "Object.h"
#include "Weapon.h"
#include "EnemyManager.h"
#include "CollisionHandler.h"
#include "ConsumablesManager.h"
#include "SpawnHandler.h"
#include "GameHandler.h"


// Main classes: drawer and player 
Drawer drawer;
Player player;

// Handlers
GameHandler gameHandler;
CollisionHandler collisionManager;
SpawnHandler spawnHandler;

// Managers 
EnemyManager enemyManager;
ConsumablesManager consumableManager;


// Various objects
Object environment;
Object m4, ak, winch;
Weapon m4a1, ak47, winchester;


// FUNCTIONS FOR GLUT IDLE FUNCTION
void aim(void) {
	player.aim();
}

void shoot(void) {
	player.shoot();
}

void idle(void) {
	player.idle();
}

void walk(void) {
	player.walk();
}

void reload(void) {
	player.reload();
}

void swap(void) {
	player.swap();
}


// Function that reset values when a new game starts
void reset(void) {

	player.reset();
	enemyManager.reset();
	consumableManager.reset();

}

// Setup function for initializing values
void setup(void) {

	// Init a game handler
	gameHandler = GameHandler();

	// Creating a new player 
	player = Player();

	// Initializing a drawer
	drawer = Drawer(0);

	// Loading scores from the Scoreboard file, so if the user want to access it he can 
	gameHandler.loadScoreboard();

	// REFERENCE ASSIGN!
	// I know its a bad practice assigning the reference to something else but i need 
	// things done fast lmao 
	drawer.player = &player;
	drawer.enemyManager = &enemyManager;
	drawer.consumablesManager = &consumableManager;
	drawer.gameHandler = &gameHandler;
	spawnHandler.consumableManager = &consumableManager;
	spawnHandler.enemyManager = &enemyManager;
	collisionManager.player = &player;
	collisionManager.handler = &spawnHandler;

	// Play title theme
	gameHandler.playTitleTheme();

}

// Function that loads some assets when starting a game to avoid lenghty loading blank screen
void loadAssets(void) {

	if (!gameHandler.assetLoaded) {

		// Creating enemy and consumables templates loading their relative models
		enemyManager.assignTemplates();
		consumableManager.assignTemplates();

		// Init object loading 
		environment.load(Color4f(), "resources/models/landscape/", "flandscape.obj", "flandscape.mtl");

		// Loading a weapon
		//m4a1.load(Color4f(), "resources/models/weapons/m16/", "m16.obj", "m16.mtl");
		ak47.load(Color4f(), "resources/models/weapons/ak47/", "m17s.obj", "ak47.mtl");
		winchester.load(Color4f(), "resources/models/weapons/winchester/", "winchester.obj", "winchester.mtl");

		// M4 Weapon Properties
		m4a1.setFirerate(45);
		m4a1.setCapacity(32);
		m4a1.reload(32);
		m4a1.setDamage(7);
		m4a1.setRecoil(1.5);
		m4a1.setType(TYPE::AUTOMATIC);
		m4a1.setSound("resources/audio/weapons/m4a1.mp3");

		// Winchester Properties
		winchester.setFirerate(60);
		winchester.setCapacity(4);
		winchester.reload(4);
		winchester.setDamage(90);
		winchester.setRecoil(15.5);
		winchester.setType(TYPE::SHOTGUN);
		winchester.setSound("resources/audio/weapons/winchester.mp3");

		// AK Properties
		ak47.setFirerate(30);
		ak47.setCapacity(42);
		ak47.reload(42);
		ak47.setDamage(12);
		ak47.setRecoil(1.5);
		ak47.setType(TYPE::AUTOMATIC);
		ak47.setSound("resources/audio/weapons/ak47.mp3");

		// Assign those weapons to the player
		player.assign(ak47);
		player.assign(winchester);

		// WTF? without this two lines the weapons do not get displayed. Thats because i forgot to load them 
		// as pointer into their relative class, so when they're drawn basically are invisible, probably because on how 
		// freeGlut handles drawing and references to object/pointers. What a clusterfuck lol
		winch.load(Color4f(), "resources/models/weapons/winchester/", "winchester.obj", "winchester.mtl");
		ak.load(Color4f(), "resources/models/weapons/ak47/", "m17s.obj", "ak47.mtl");

		// Set flag 
		gameHandler.assetLoaded = true;
	
	}
}

void mouseMove(int x, int y) {

	if (gameHandler.state == STATE::GAME) {

		// Raycast if moving the mouse to upgrade position (if needed)
		player.raycast();

		// If the player is not doing anything, restore the idle animation
		if (!player.shootstate && !player.aimstate && !player.walkstate && !player.reloadstate && player.idlestate) glutIdleFunc(idle);

		// Computing the new view centers
		player.look(x, y);
	
	}

	// redisplay the scene
	glutPostRedisplay();

}

void mouseButton(int button, int state, int x, int y) {

	// Handling mouse buttons: right is for aiming, left is for shooting.
	// CASE RIGHT BUTTON (AIM)
	if (gameHandler.state == STATE::GAME) {
		switch (button) {

			// CASE: AIMING
		case GLUT_RIGHT_BUTTON:

			// If we're pressing it
			if (state == GLUT_DOWN) {

				// If the player isn't reloading (can't aim while reloading :p)
				if (!player.reloadstate) {

					// Disable idle and activate aimstate
					player.aimtransition = true;
					player.idlestate = false;
					player.aimstate = true;

					// Set the idle func 
					glutIdleFunc(aim);

				}

			}

			// If we released it
			else if (state == GLUT_UP) {

				// If the player is not reloading (can't change aim status while reloading :p)
				if (!player.reloadstate) {

					// Disable aimstate
					player.aimtransition = true;
					player.aimstate = false;

					// Set the aim func again to reset the aim animation
					glutIdleFunc(aim);

				}

			}

			// stop here
			break;

			// CASE: SHOOTING
		case GLUT_LEFT_BUTTON:

			// If the button is being pressed
			if (state == GLUT_DOWN) {

				// If the player isn't reloading (can't shoot while reloading :p)
				// and isnt transitioning into the aim position
				if (!player.reloadstate && !player.aimtransition) {

					// Idle is false, shootstate is true
					player.idlestate = false;
					player.shootstate = true;

					// Raycast projection of the mouse click
					player.raycast();

					// Set the idle func to perform the animation 
					glutIdleFunc(shoot);

				}

			}

			// BUTTON UP
			else if (state == GLUT_UP) {

				// Stop shooting: we do not call the shoot function again because when we stop shooting 
				// we need to be able to still aim walk etc
				player.shootstate = false;

				//TMP reset the shot
				player.inventory.weapons.at(player.currentweapon).singleShotWeaponFired = false;

				// Reset the projection 
				player.raycaster.reset();

				// If while shooting the player was aimign or transitioning an aim, reset the aim 
				// animation with the aim idle func, otherwise we just idling
				if (player.aimstate || player.aimtransition) glutIdleFunc(aim);
				else player.idlestate = true;

			}

			// stop here
			break;

			// CASE: SWITCHING WEAPON UP
		case 3:

			// Assuring we're not aiming, shooting or reloading
			if (!player.shootstate && !player.aimstate && !player.reloadstate) {

				player.idlestate = false;
				player.swapstate = true;
				glutIdleFunc(swap);

			}

			break;

			// CASE: SWITCHING WEAPON DOWN
		case 4:

			// Assuring we're not aiming, shooting or reloading
			if (!player.shootstate && !player.aimstate && !player.reloadstate) {

				player.idlestate = false;
				player.swapstate = true;
				glutIdleFunc(swap);

			}

			break;

		}
	}

}

// Function that performs some operations when a special key is pressed
void pressSpecialKey(int key, int xx, int yy) {


	// Using special keys we navigate through the menu
	switch (key) {

		case GLUT_KEY_UP:
			gameHandler.menuPrevSelection();
			break;


		case GLUT_KEY_DOWN:
			gameHandler.menuNextSelection();
			break;
	}

}

// Function that performs some operations when a key is pressed
void pressKey(unsigned char key, int xx, int yy) {

	std::cout << key << std::endl;

	// Case switch with keyboard directions
	if (gameHandler.state == STATE::GAME) {
		switch (key) {

		case 'W':
		case 'w':

			// If the player isn't:
			// Shooting, aiming and reloading
			// AND if the player is in idle state: 
			if (!player.shootstate && !player.aimstate && !player.reloadstate && player.idlestate) {

				// Disable idlestate and enable walking state
				player.idlestate = false;
				player.walkstate = true;

				// Set the default idle func as the walking (to perform the animation)
				glutIdleFunc(walk);
			}

			// Regardless of the animation or not we move in the desired direction 
			// (this to, i.e: moving while shooting etc).
			player.changePosition(DIRECTION::FORWARD);
			break;

		case 'S':
		case 's':

			// SAME AS ABOVE
			if (!player.shootstate && !player.aimstate && !player.reloadstate && player.idlestate) {
				player.idlestate = false;
				player.walkstate = true;
				glutIdleFunc(walk);
			}
			player.changePosition(DIRECTION::BACKWARD);
			break;

		case 'A':
		case 'a':

			// When moving left or right we dont want to play no animation, so we just change
			// the direction
			player.changePosition(DIRECTION::LEFT);
			break;

		case 'D':
		case 'd':

			// SAME AS ABOVE
			player.changePosition(DIRECTION::RIGHT);
			break;

		case 'R':
		case 'r':

			// If the player isn't: shooting, aiming, walking and if its idle
			if (!player.shootstate && !player.aimstate && !player.walkstate && player.idlestate) {

				// idlestate is false and we activate the reloadstate
				player.idlestate = false;
				player.reloadstate = true;

				// Set the current idle func
				glutIdleFunc(reload);
			}
			break;

		}
	}
	else {

		// Switch menu keys
		switch (key) {

			case (char)13:

				// Enter case: we need to swap state
				if (gameHandler.state != STATE::GAME) {

					// If the state is gameover, we need to change state only when the min size 
					// of the string name has been filled to write the name/score pair into a file.
					if (gameHandler.state == STATE::GAMEOVER) {
						
						// If a min string size is reached
						if (player.name.reachedMinSize()) {

							// HANDLE SCORE INSERT INTO FILE
							// We create a new score object and insert into the scoreboard.
							// We also save it directly to avoid score losing due to unpredictable app closing
							ScoreInfo newScore = ScoreInfo(player.name.data, gameHandler.difficulty, enemyManager.totalkill);
							gameHandler.insertNewScore(newScore);
							gameHandler.saveScoreboard();
							reset();
							gameHandler.enterNewState();

						}
					}
					else{

						// ELSE we simply enter a new state
						gameHandler.enterNewState();

					}

				}
				break;

			case (char)27:

				// Esc case: we need to swap state (going back) only if in difficulty selection
				if (gameHandler.state == STATE::DIFFICULTY) gameHandler.restoreTitleState();
				break;

			case (char)8:

				// Delete Key case: If we're in the gameover screen, we must be able to delete the last
				// inserted key into our name. We just call the deleteKey from player
				if (gameHandler.state == STATE::GAMEOVER) {

					player.name.deleteStringKey();

				}

				// Play key pressed sound
				gameHandler.playKeySound();

				break;

			default:

				// Default case: in this case we only need to get the user input while in gameover mode.
				// Thats because at the end of the game the user needs to input is name for the score saving.
				// We just call the addNameKey of player
				if (gameHandler.state == STATE::GAMEOVER) {

					player.name.addStringKey(key);

				}

				// Play key pressed sound
				gameHandler.playKeySound();

				break;

		}

	}

}

void releaseKey(unsigned char key, int x, int y) {

	// Case switch with keyboard directions: this action 
	// will be performed when the key is switched up, so we just revert
	// the states
	if (gameHandler.state == STATE::GAME) {
		switch (key) {

		case 'W':
		case 'w':
			player.walkstate = false;
			player.idlestate = true;
			glutIdleFunc(walk);
			break;

		case 'S':
		case 's':
			player.walkstate = false;
			player.idlestate = true;
			glutIdleFunc(walk);
			break;

		case 'A':
		case 'a':
			player.walkstate = false;
			glutIdleFunc(walk);
			break;

		case 'D':
		case 'd':
			player.walkstate = false;
			glutIdleFunc(walk);
			break;

		}
	}


}


void drawScene(void) {

	switch (gameHandler.state) {

		// Titlescreen 
		case STATE::TITLE:

			drawer.drawTitleScreen();
			break;

		// Difficulty selection
		case STATE::DIFFICULTY:

			drawer.drawDifficulty();
			break;

		// Loading screen 
		case STATE::LOADING:

			// Drawing loading screen
			drawer.drawLoading();

			// Loading assets and setting variables
			loadAssets();
			drawer.loadGameResources();
			enemyManager.difficulty = gameHandler.difficulty;

			// Switching state to game mode
			gameHandler.state = STATE::GAME;

			// Play game sound
			gameHandler.playGameTheme();

			// going back
			break;

		// About page
		case STATE::ABOUT:

			drawer.drawAbout();
			break;

		// High Score page
		case STATE::SCORES:

			drawer.drawScores();
			break;

		// Game loop
		case STATE::GAME:

			// Clearing procedures 
			drawer.clear();

			// Updating the camera 
			player.updateView();

			// Drawing the scene
			drawer.drawScene(environment);

			// Check for collisions 
			collisionManager.checkCollisions();
		
			// After the collision, we check if the player has still enough hp to continue the game
			if (player.alive) {

				// Managing enemy routines (moving, check death)
				enemyManager.manageEnemies(player.getCurrentPosition());

				// Draw the rest of the scene (needs to be on front on everything)
				drawer.drawUtil();

				// We then call the spawn function to check if a new enemy must be spawn
				spawnHandler.spawnEnemy();

			}
			else {

				// Else the player is dead, set the gameover state
				gameHandler.state = STATE::GAMEOVER;
				gameHandler.playGameOverTheme();

			}


			// Swap buffer for drawing
			glutSwapBuffers();

			break;

		// Gameover page
		case STATE::GAMEOVER:
			
			// Draw gameover screen
			drawer.drawGameOver();
			break;

		// Exit page 
		case STATE::EXIT:

			// Save scoreboard and exit 
			glutDestroyWindow(0);
			exit(0);

	}

}

void resize(int w, int h) {

	// Setting ratio and new window size
	float ratio = w * 1.0 / h;

	// Updating camera values 
	player.view.update_window(w, h);

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

void update(int value) {

	// Timer function in which we redisplay the content of the screen each 1000/60 
	// msec, meaning that we want to achieve a 60fps configuration.
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, update, 0);

	// updating player tick: resetting if equal to 60, increasing if not
	player.updateTick();

	// Aswell updating the enemy manger tick, resetting it at 240;
	enemyManager.updateTick();

	// Updating the drawer tick, will draw how much time has passed since the start
	drawer.updateTick();

}



void main(int argc, char** argv) {

	// Init procedures
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920, 1080);
	glutMouseFunc(mouseButton);

	// Creating a window 
	glutCreateWindow("GALACTIC MEGA SLAUGHTER.cpp");

	// Func callback
	glutReshapeFunc(resize);
	glutDisplayFunc(drawScene);
	glutSpecialFunc(pressSpecialKey);
	glutKeyboardFunc(pressKey);
	glutKeyboardUpFunc(releaseKey);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseButton);
	glutTimerFunc(100, update, 0);

	// Setup func 
	setup();

	// start loop
	glutMainLoop();

}

