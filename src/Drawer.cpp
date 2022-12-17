#include "Drawer.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that draw a scene in a specified order
void Drawer::drawScene(Object& environment) {

	// NOTE: This function follow a very specific order of rendering. 
	// The first thing we draw is the skybox since it is a cubemap and we need to be mapped inside of it.
	// Then the rest!
	this->enableFog(fogtype::LIGHT);
	this->draw_skybox();
	this->draw_terrain();
	this->drawEnvironment(environment);
	this->draw_cur_weapon();

}

// Function that handles the drawing of the titlescreen
void Drawer::drawTitleScreen(void) {

	int MENU_ENTRY = this->gameHandler->menuEntry;

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.title);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Lines variables 
	int x1, y1, x2, y2;

	// Switching menu entry
	switch (MENU_ENTRY) {
		
		case 0:
			x1 = 780 - 50;
			x2 = 630;
			y1 = y2 = 735;
			break;

		case 1:
			x1 = 750 - 50;
			x2 = 660;
			y1 = y2 = 815;
			break;

		case 2:
			x1 = 780 - 50;
			x2 = 620;
			y1 = y2 = 890;
			break;

		case 3:
			x1 = 845 - 50;
			x2 = 585;
			y1 = y2 = 975;
			break;

	}

	// Drawing lines selection
	glLineWidth(4);
	glColor3f(1, 1, 1);

	// Displaying the lines twice in a second to give a flashing effect
	if (this->tick <= 30) {

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// First line 
		glBegin(GL_LINES);
		glVertex2f(x1 - 50, y1);
		glVertex2f(x1, y1);
		glEnd();

		// Second line
		glBegin(GL_LINES);
		glVertex2f(x2 + 600, y2);
		glVertex2f(x2 + 650, y2);
		glEnd();


		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}

	// Restore 
	glLineWidth(1);

	// Restore projection
	this->restorePerspectiveProjection();

	// Swap buffer for drawing
	glutSwapBuffers();


}

// Function that draws a difficulty screen to select 
void Drawer::drawDifficulty(void) {

	int DIFFICULTY_ENTRY = this->gameHandler->difficultyEntry;

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.difficulty);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Lines variables 
	int x1, y1, x2, y2;

	// Switching menu entry
	switch (DIFFICULTY_ENTRY) {

	case 0:
		x1 = 800 - 50;
		x2 = 600;
		y1 = y2 = 735;
		this->gameHandler->difficulty = GAMEMODE::EASY;
		break;

	case 1:
		x1 = 750 - 50;
		x2 = 660;
		y1 = y2 = 815;
		this->gameHandler->difficulty = GAMEMODE::NORMAL;
		break;

	case 2:
		x1 = 780 - 50;
		x2 = 620;
		y1 = y2 = 890;
		this->gameHandler->difficulty = GAMEMODE::HARD;
		break;

	case 3:
		x1 = 745 - 50;
		x2 = 640;
		y1 = y2 = 975;
		this->gameHandler->difficulty = GAMEMODE::MADNESS;
		break;

	}

	// Drawing lines selection
	glLineWidth(4);
	glColor3f(1, 1, 1);

	
	// Displaying the lines twice in a second to give a flashing effect
	if (this->tick <= 30) {

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// First line 
		glBegin(GL_LINES);
		glVertex2f(x1 - 50, y1);
		glVertex2f(x1, y1);
		glEnd();

		// Second line
		glBegin(GL_LINES);
		glVertex2f(x2 + 600, y2);
		glVertex2f(x2 + 650, y2);
		glEnd();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

	}

	// Restore 
	glLineWidth(1);

	// Restore projection
	this->restorePerspectiveProjection();

	// Swap buffer for drawing
	glutSwapBuffers();

}


// Function that draws the loading screen
void Drawer::drawLoading(void) {

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.loading);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Restore projection
	this->restorePerspectiveProjection();

	// Swap buffer for drawing
	glutSwapBuffers();


}

// Function that draws the about section
void Drawer::drawAbout(void) {

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.about);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Restore projection
	this->restorePerspectiveProjection();

	// Swap buffer for drawing
	glutSwapBuffers();



}

// Function that draws the high score section
void Drawer::drawScores(void) {

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.scoreboard);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Restore projection
	this->restorePerspectiveProjection();

	// Drawing score on the screen
	// DRAW SCORE ON THE SCREEN
	// Disabling flags
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// buffer to print info in
	char buffer[64];

	// Linewidth
	glLineWidth(5);
	float y_offset = 0.12;

	for (int i = 0; i < this->gameHandler->scoreHandler.scores.size(); i++) {

		// Fetching values 
		std::string name = this->gameHandler->scoreHandler.scores.at(i).name;
		std::string difficulty = gameHandler->getDifficultyString(this->gameHandler->scoreHandler.scores.at(i).difficulty);
		std::string score = std::to_string(this->gameHandler->scoreHandler.scores.at(i).score);

		// Printing into buf
		memset(buffer, 0, sizeof buffer);
		snprintf(buffer, sizeof buffer, "%s", score.c_str());
		fontmanager.drawStrokeFontInterface(Vector3f(-0.8, 0.5 - i*y_offset, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0006);

		memset(buffer, 0, sizeof buffer);
		snprintf(buffer, sizeof buffer, "%s", name.c_str());
		fontmanager.drawStrokeFontInterface(Vector3f(-0.3, 0.5 - i * y_offset, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0006);
		
		memset(buffer, 0, sizeof buffer);
		snprintf(buffer, sizeof buffer, "%s" ,difficulty.c_str());
		fontmanager.drawStrokeFontInterface(Vector3f(0.35, 0.5 - i * y_offset, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0006);

	}

	// Enabling flags AND restoring lw
	glLineWidth(1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Swap buffer for drawing
	glutSwapBuffers();

}


// Function that draws the game over section
void Drawer::drawGameOver(void) {

	// Clearing procedures 
	this->clear();

	// Setting orthographic projection
	this->setOrthographicProjection();

	// Enable textures
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->menu.gameover);

	// Draw the menu 
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(0, 0);
	glTexCoord2f(0, 0); glVertex2f(0, this->hudh);
	glTexCoord2f(1, 0); glVertex2f(this->hudw, this->hudh);
	glTexCoord2f(1, 1);	glVertex2f(this->hudw, 0);
	glEnd();

	// Disable menu texture
	glDisable(GL_TEXTURE_2D);

	// Restore projection
	this->restorePerspectiveProjection();

	// DRAW SCORE ON THE SCREEN
	// Disabling flags
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// buffer to print info in
	char buffer[64];
	int score = this->enemyManager->totalkill;

	// Linewidth
	glLineWidth(5);

	// printing info in buffer and displaying
	memset(buffer, 0, sizeof buffer);
	snprintf(buffer, sizeof buffer, "SLAUGHTER SCORE: %d", int(score));
	fontmanager.drawStrokeFontInterface(Vector3f(-0.40, -0.3, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0005);

	memset(buffer, 0, sizeof buffer);
	snprintf(buffer, sizeof buffer, "TIME ALIVE: %d", this->secondElapsed);
	fontmanager.drawStrokeFontInterface(Vector3f(-0.40, -0.4, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1.0, 1.0, 1.0), 0.0005);

	memset(buffer, 0, sizeof buffer);
	snprintf(buffer, sizeof buffer, "INSERT NAME: %s", this->player->name.data.c_str());
	fontmanager.drawStrokeFontInterface(Vector3f(-0.40, -0.5, -1.5), Vector3f(0.0, 0, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1.0, 1.0, 1.0), 0.0005);

	// Enabling flags AND restoring lw
	glLineWidth(1);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Swap buffer for drawing
	glutSwapBuffers();

}

void Drawer::drawInterface() {

	// Disabling effects
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	// Drawing an opaque rectangle to put the info on:
	// we do this in orthographic projection to draw a simil 2d-hud
	this->setOrthographicProjection();

	// Loading a new stack and pushing a panel into it
	glPushMatrix();
	glLoadIdentity();

	// Cross quad
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->interfaceHP);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(50.0, this->hudh - 90);
	glTexCoord2f(0, 0); glVertex2f(50.0, this->hudh - 20);
	glTexCoord2f(1, 0); glVertex2f(120.0, this->hudh - 20);
	glTexCoord2f(1, 1);	glVertex2f(120.0, this->hudh - 90);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Bullet quad
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->interfaceBullet);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(320.0, this->hudh - 90);
	glTexCoord2f(0, 0); glVertex2f(320.0, this->hudh - 20);
	glTexCoord2f(1, 0); glVertex2f(390.0, this->hudh - 20);
	glTexCoord2f(1, 1);	glVertex2f(390.0, this->hudh - 90);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Difficulty quad
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->interfaceSkull);
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glBindTexture(GL_TEXTURE_2D, this->interfaceSkull);
	glTexCoord2f(0, 1); glVertex2f(650.0, this->hudh - 90);
	glTexCoord2f(0, 0); glVertex2f(650.0, this->hudh - 20);
	glTexCoord2f(1, 0); glVertex2f(720.0, this->hudh - 20);
	glTexCoord2f(1, 1);	glVertex2f(720.0, this->hudh - 90);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	// Restoring perspective
	this->restorePerspectiveProjection();

	// Disabling texturing
	glDisable(GL_TEXTURE_2D);

	// Getting variables to shorten things off
	int weaponAmmo = this->player->inventory.weapons.at(this->player->currentweapon).ammo;
	int backpackAmmo = this->player->inventory.ammo;
	int hp = this->player->hp;
	int score = this->enemyManager->totalkill;
	
	// Buffer where to print strings inside 
	glLineWidth(7);
	char buffer[64];

	snprintf(buffer, sizeof buffer, "%d", int(hp));
	fontmanager.drawStrokeFontInterface(Vector3f(-0.50, -0.3, -0.5), Vector3f(0.0, 1, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0002);

	memset(buffer, 0, sizeof buffer);
	snprintf(buffer, sizeof buffer, "%d/%d", int(weaponAmmo), int(backpackAmmo));
	fontmanager.drawStrokeFontInterface(Vector3f(-0.33, -0.3, -0.5), Vector3f(0.0, 1, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1.0, 1.0, 1.0), 0.0002);

	memset(buffer, 0, sizeof buffer);
	snprintf(buffer, sizeof buffer, "%d", int(score));
	fontmanager.drawStrokeFontInterface(Vector3f(-0.12, -0.3, -0.5), Vector3f(0.0, 1, 0.0), 0, GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1.0), 0.0002);

	// Re enabling flags
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
}

// Function that draws the blood on the screen if the player is being hit
void Drawer::drawBlood(void) {


	if (this->player->isHit) {

		// Drawing an opaque rectangle to put the info on:
		// we do this in orthographic projection to draw a simil 2d-hud
		this->setOrthographicProjection();

		// Disabling effects
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Loading a new stack and pushing a panel into it
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS);
		glPushAttrib(GL_COLOR_BUFFER_BIT);
		glColor4f(1.0f, 0.0f, 0.0, 0.5);
		glPopAttrib();
		glVertex2f(0, 0);
		glVertex2f(0, this->hudh);
		glVertex2f(this->hudw, this->hudh);
		glVertex2f(this->hudw, 0);
		glEnd();
		glPopMatrix();

		// Restoring perspective
		this->restorePerspectiveProjection();

		// re enable
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		// Set hit to false
		this->player->isHit = false;

	}




}

// Function that given an object will draw it on screen.
void Drawer::drawObject(Object &object, Vector3f position, Vector3f rotaxes, float scale, float angle) {

	// Pushing a new matrix onto the stack and applying transformation to the objects.
	// Note that, the transformation in OPENGL are applied in the inverse order since the matrix mult.
	// if from the right: that means we're applying first a scaling, then rotating then translating.
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(angle, rotaxes.x, rotaxes.y, rotaxes.z);
	glScalef(scale, scale, scale);
	object.render();
	glPopMatrix();

}

void Drawer::drawEnvironment(Object& environment) {

	// Classic code to check if we reached a lim and then inverse it (used in many other part of this code)
	if (this->envfloat) {

		if (this->envy <= 1.5) {
			this->envy += 0.015;
			if (this->envy >= 1.) this->envfloat = false;
		
		}

	}
	else {

		if (this->envy >= 0.) {

			this->envy -= 0.015;
			if (this->envy <= 0.) this->envfloat = true;

		}


	}

	// LEFT SIDE
	this->drawObject(environment, Vector3f(-150.0, -7.0 + envy, -330.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-150.0, -7.5 - envy, -150.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-150.0, -7.5 + envy, -230.0), Vector3f(0.0, 1.0, 0), 0.2, -85);

	// BOTTOM SIDE
	this->drawObject(environment, Vector3f(-75.0, -7.0 + envy, -65.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(0.0, -7.0 - envy, -65.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(75, -7.0 + envy, -65.0), Vector3f(0.0, 1.0, 0), 0.2, -85);

	// RIGHT SIDE
	this->drawObject(environment, Vector3f(85.0, -7.5 - envy, -150.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(85.0, -7.0 + envy, -300.0), Vector3f(0.0, 1.0, 0), 0.2, -85);

	// TOP SIDE
	this->drawObject(environment, Vector3f(90.0, -7.0 + envy, -370.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-10.0, -7.3 + envy, -390.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-100.0, -7.5 - envy, -370.0), Vector3f(0.0, 1.0, 0), 0.2, -85);

	/// FAR AWAYY
	this->drawObject(environment, Vector3f(-150.0, -7.5 + envy, -530.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(85.0, -7.5 - envy, -450.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-75.0, -7.0 + envy, -565.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(0.0, -7.0 - envy, -565.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(75, -7.0 + envy, -565.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(60.0, -7.0 + envy, -570.0), Vector3f(0.0, 1.0, 0), 0.2, -85);
	this->drawObject(environment, Vector3f(-120.0, -7.5 - envy, -570.0), Vector3f(0.0, 1.0, 0), 0.2, -85);

}


// Function that with draws the current alive enemies in the scene
void Drawer::drawEnemy(void) {

	// If there are enemies 
	if (this->enemyManager->Enemies.size() > 0) {

		// For each enemy in the enemy manager enemies list 
		for (int i = 0; i < this->enemyManager->Enemies.size(); i++) {

			// Declaring the scale and position for the sake of avoiding useless code repetition later
			float enescale = this->enemyManager->Enemies.at(i).scale;
			float eneposx = this->enemyManager->Enemies.at(i).posx;
			float eneposy = this->enemyManager->Enemies.at(i).posy;
			float eneposz = this->enemyManager->Enemies.at(i).posz;
			float eneangle = this->enemyManager->Enemies.at(i).dirangle;

			// Check the percentual of HP BAR scaling based on the difficulty 
			float xhpscale = 0;
			float xhptransl = 0;
			switch (this->gameHandler->difficulty) {

				case GAMEMODE::EASY:
					xhptransl = -40;
					xhpscale = 0.25;	
					break;

				case GAMEMODE::NORMAL:
					xhptransl = -80;
					xhpscale = 0.08;
					break;

				case GAMEMODE::HARD:
					xhptransl = -110;
					xhpscale = 0.04;
					break;

				case GAMEMODE::MADNESS:
					xhptransl = -230;
					xhpscale = 0.02;
					break;

			}

			// Push a new matrix in the stack and translate/scale/rotate the enemy
			// based on his position and reach to the player 
			glPushMatrix();
			glTranslatef(eneposx, eneposy, eneposz);
			glScalef(enescale, enescale, enescale);
			glRotatef(eneangle, 0.0, 1.0, 0.0);

			// Draw healt bar and healt number, as usual for sak of simplicity 
			// values of rotations translation etc are hard coded.
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
			glScalef(xhpscale, 1.0, 3.0);
			glTranslatef(xhptransl, 10, 2);
			glRotatef(-90, 0, 0, 1);
			glBegin(GL_QUADS);

			// Green health bar

			glPushAttrib(GL_COLOR_BUFFER_BIT);
			glColor3f(0, 1, 0.2);
			glPopAttrib();

			// The health bar is simply a quad based on the remaining hp of the enemy
			if (this->enemyManager->Enemies.at(i).hp > 0) {
				glVertex2f(1, this->enemyManager->Enemies.at(i).hp);
				glVertex2f(0, this->enemyManager->Enemies.at(i).hp);
				glVertex2f(0, 0);
				glVertex2f(1, 0);
			}

			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);

			// Drawing the hp remained 
			glLineWidth(3);
			char buffer[64];
			int ret = snprintf(buffer, sizeof buffer, "%.1f", this->enemyManager->Enemies.at(i).hp);
			float randred = this->rng.getRng(0, 1);
			float randgreen = this->rng.getRng(0, 1);
			float randblue = this->rng.getRng(0, 1);
			this->fontmanager.drawStrokeFont(Vector3f(- 2, 7, 2), GLUT_STROKE_ROMAN, buffer, Color4f(1, 1, 1, 1), 0.01);
			
			// Render the enemy based on the current shot state (hit or not)
			if (this->enemyManager->Enemies.at(i).beingShot) {
				// If the enemy is being shot, draw the hit model and reset his state
				this->enemyManager->ProbeTemplates.at(this->enemyManager->Enemies.at(i).getID()).second->draw();
				this->enemyManager->Enemies.at(i).beingShot = false;
			}
			// Else draw the normal model
			else {
				this->enemyManager->ProbeTemplates.at(this->enemyManager->Enemies.at(i).getID()).first->draw();
			}
			
			// Pop the matrix 
			glPopMatrix();

		}

	}

}

// Function that will draw consumables on the ground with some animation 
void Drawer::drawConsumables(void) {

	this->draw_ammobox();
	this->draw_healthbox();

}




// Function that draws the remaining elements in the scene that needs to stay on front
void Drawer::drawUtil(void) {

	this->drawBlood();
	this->drawEnemy();
	this->drawConsumables();
	this->drawInterface();	
	this->draw_gunshot();
	this->draw_shoot();
	this->draw_crosshair();

}



// Function that enables fog given a fog type
void Drawer::enableFog(fogtype TYPE) {


	std::vector<float> fogcol(4, 0);
	//enable the fog
	glEnable(GL_FOG);

	//set the fog mode to GL_EXP2
	glFogi(GL_FOG_MODE, GL_EXP2);

	//set the fog color to
	switch (TYPE) {

	case fogtype::LIGHT:
		fogcol = { 0.4, 0.4, 0.4, 0.2 };
		break;

	case fogtype::STANDARD:
		fogcol = { 0.6, 0.6, 0.6, 0.4 };
		break;

	case fogtype::DARK:
		fogcol = { 0.8, 0.8, 0.8, 0.6 };
		break;

	}

	// Assign the right fogcol
	glFogfv(GL_FOG_COLOR, fogcol.data());

	//set the density to the
	glFogf(GL_FOG_DENSITY, this->fogdensity);

	// set the fog to look the
	glHint(GL_FOG_HINT, GL_NICEST);

}

// Function that assures cleaning buffer procedures/identity
void Drawer::clear(void) {

	// Procedures that clears the buffer and restores the transformation matrix to identity
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

}

// Function that set the Perspective projection as the main projection type
void Drawer::setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection: we fix an widht/height of 640/480 for resizing automatically the 
	// elements of the scene up to 1080p 
	gluOrtho2D(0, this->hudw, this->hudh, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);

}


// Function that set the Orthographic projection as the main projection type
void Drawer::restorePerspectiveProjection(void) {

	// Swith to projection mode 
	glMatrixMode(GL_PROJECTION);

	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);

}

// Function that does update the internal tick 
void Drawer::updateTick(void) {

	// Increase the second elpased based on a full second cycle
	if (gameHandler->state != STATE::GAMEOVER) {
		if (this->tick > 60.0f) {
			this->tick = 0;
			this->secondElapsed++;
		}
		else {
			this->tick++;
		}
	}
	else {
		this->tick = 0;
	}

}

// Function that, once the game has started through the menu selection, loads the 
// resources needed by the game to run properly to avoid useless loading in the menu screen
void Drawer::loadGameResources(void) {

	// Loading ground textures
	this->terrain = this->texloader.load("resources/textures/ground/ground.jpg", GROUND);

	// Loading gunshot texture
	this->gunshot = this->texloader.load("resources/textures/effects/weapon/muzzle.png", STANDARD);

	// Skybox textures
	this->skybox[SKY_FRONT] = this->texloader.load("resources/textures/skybox/space/front.jpg", SKY);
	this->skybox[SKY_RIGHT] = this->texloader.load("resources/textures/skybox/space/right.jpg", SKY);
	this->skybox[SKY_LEFT] = this->texloader.load("resources/textures/skybox/space/left.jpg", SKY);
	this->skybox[SKY_BACK] = this->texloader.load("resources/textures/skybox/space/back.jpg", SKY);
	this->skybox[SKY_UP] = this->texloader.load("resources/textures/skybox/space/top.jpg", SKY);
	this->skybox[SKY_DOWN] = this->texloader.load("resources/textures/skybox/space/bottom.jpg", SKY);

	// Load interface textures


	/*
	this->skybox[SKY_FRONT] = this->texloader.load("resources/textures/skybox/titan/front.png", SKY);
	this->skybox[SKY_RIGHT] = this->texloader.load("resources/textures/skybox/titan/right.png", SKY);
	this->skybox[SKY_LEFT] = this->texloader.load("resources/textures/skybox/titan/left.png", SKY);
	this->skybox[SKY_BACK] = this->texloader.load("resources/textures/skybox/titan/back.png", SKY);
	this->skybox[SKY_UP] = this->texloader.load("resources/textures/skybox/titan/top.png", SKY);
	this->skybox[SKY_DOWN] = this->texloader.load("resources/textures/skybox/titan/bottom.png", SKY);
	*/


}


///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

void Drawer::init(void) {

	// Init TextureManager class for loading texture 
	this->texloader = TextureLoader();
	
	// Setting time vars
	this->tick = 1;
	this->secondElapsed = 0;

	// Loading environments vars
	this->envfloat = false;
	this->envy = 1.;

	// Load the first two menu textures 
	this->menu.title = this->texloader.load("resources/textures/titlescreen/titlescreen.png", STANDARD);
	this->menu.loading = this->texloader.load("resources/textures/titlescreen/loading.png", STANDARD);
	this->menu.difficulty = this->texloader.load("resources/textures/titlescreen/difficulty.png", STANDARD);
	this->menu.about = this->texloader.load("resources/textures/titlescreen/about.png", STANDARD);
	this->menu.scoreboard = this->texloader.load("resources/textures/titlescreen/plain.png", STANDARD);
	this->menu.gameover = this->texloader.load("resources/textures/titlescreen/gameover.png", STANDARD);

	this->interfaceHP = this->texloader.load("resources/textures/interface/cross.png", STANDARD);
	this->interfaceSkull = this->texloader.load("resources/textures/interface/skull.png", STANDARD);
	this->interfaceBullet = this->texloader.load("resources/textures/interface/bullet.png", STANDARD);

	// Initializing setup routines 
	this->setup();

}

// Function that initializes various gl flags
void Drawer::setup(void) {

	// Enabling blend, alpha channel, depth test, culling disabling cursonr and setting cull to back faces
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glutSetCursor(GLUT_CURSOR_NONE);
	glCullFace(GL_BACK);

	// Useless since we do not use shaders
	//glShadeModel(GL_SMOOTH);

	// Clearing background color and setting up to black
	glClearColor(1.0, 1.0, 1.0, 1.0);

}

void Drawer::draw_terrain(void) {
	
	// We make the depth function pass the test always, avoiding problems when 
	// looking down in the this->player->view
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_TEXTURE_2D);

	// Setting a white color for the polygons and binding the texture terrain
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor3f(0.5f, 0.5f, 0.5f);
	glPopAttrib();

	glBindTexture(GL_TEXTURE_2D, this->terrain);

	// For z starting at zlim and increasing  by tilesize
	for (float z = this->zlim; z >= -this->zlim; z -= tilesize) {
	
		// We begin drawing triangle strip that will be used to apply texture on them.
		glBegin(GL_TRIANGLE_STRIP);

		// Using tmp variables to store the tex coordinate: since the u,v system vary in [0,1], 
		// we'll use that information to draw, each time, a row of triangles and then texture them.
		// Infact, xcord will vary between 0 and xlim*2/tilesize (i.e: xlim = 100 and tilesize = 2.5?
		// Then we'll have 200/2.5 = 80 tiles per row, and x will vary between 0 and 80.
		int xcord = 0;
		int ycord[2] = { 0,1 };

		// Following the same logic as zlim but for z: we start from minus xlim to begin drawing in the 
		// left bottom corne of the scene.
		for (float x = -this->xlim; x <= this->xlim; x += this->tilesize) {

			// Drawing triangles and applying texture to them
			glTexCoord2f(xcord, ycord[0]); glVertex3f(x, 0, z - tilesize);
			glTexCoord2f(xcord, ycord[1]); glVertex3f(x, 0, z);
			xcord++;

		}

		glEnd();

	}

	// Going back to GL_LESS as depthfunc and disabling texturing
	glDepthFunc(GL_LESS);
	glDisable(GL_TEXTURE_2D);

}


void Drawer::draw_skybox() {

	// Function that draw a skybox cube (using cubemaps) around the scene. This function is really simple tbh, 
	// It just maps texture with the side of a cube.
	// We disable culling and depth test to avoid problem with the drawing
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	float D = this->skyboxdim;
	
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor4f(1, 1, 1, 1);
	glPopAttrib(); 

	glEnable(GL_TEXTURE_2D);

	/* Sides */
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(1, 1); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(1, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 1); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glEnd();


	/* Top and Bottom */
	glBindTexture(GL_TEXTURE_2D, skybox[SKY_UP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(-D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(D + this->player->view.eyex, D + this->player->view.eyey, -D + this->player->view.eyez);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKY_DOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glTexCoord2f(0, 1); glVertex3f(-D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(0, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, D + this->player->view.eyez);
	glTexCoord2f(1, 0); glVertex3f(D + this->player->view.eyex, -D + this->player->view.eyey, -D + this->player->view.eyez);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Re-enable 
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

}

// Function that draw the current player weapon 
void Drawer::draw_cur_weapon(void) {

	// Simply call render on the current player weapon: we reset the transformation stack
	glPushMatrix();
	glLoadIdentity();

	// We use an hard-coded translate in combination with the aim values (updated when the weapon 
	// is in an aim state or not) to draw an animation regarding the model. We then scale it and 
	// Rotate appropraitely.
	// +0.0161 if want to take the weapon up aligned
	// Fetching variables 
	float aimx = this->player->inventory.weapons.at(this->player->currentweapon).aimx;
	float aimy = this->player->inventory.weapons.at(this->player->currentweapon).aimy;
	float aimz = this->player->inventory.weapons.at(this->player->currentweapon).aimz;
	float aimangle = this->player->inventory.weapons.at(this->player->currentweapon).aimangle;
	float reloadangle = this->player->inventory.weapons.at(this->player->currentweapon).reloadangle;
	float shootz = this->player->inventory.weapons.at(this->player->currentweapon).shootz;
	float walkz = this->player->inventory.weapons.at(this->player->currentweapon).walkz;
	float reloady = this->player->inventory.weapons.at(this->player->currentweapon).reloady;

	// Translate, change color, scale and rotate
	glTranslatef(0.12 - aimx, -0.1 + aimy - (reloadangle/200), -0.22 + aimz + shootz + walkz);
	
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.23, 0.27);
	glPopAttrib();

	glScalef(0.38, 0.38, 0.38);
	glRotatef(-170 - aimangle + reloadangle, 0.0, 100, 7.0);

	// We then render the weapon
	this->player->inventory.weapons.at(this->player->currentweapon).render();
	glPopMatrix();

}

// Function that draw the gunshot animation if shooting 
void Drawer::draw_gunshot(void) {

	// If the player is shooting and have hammo
	int curAmmo = this->player->inventory.weapons.at(this->player->currentweapon).ammo;

	// Getting the fire rate of the current weapon 
	int currate = this->player->tick % int(round((60 / this->player->inventory.weapons.at(this->player->currentweapon).firerate / 2)));

	// Gettin shoot fired from a single shoot weapon
	bool shotFired = this->player->inventory.weapons.at(this->player->currentweapon).singleShotWeaponFired;

	// Scale factor for the gunshot texture, vary on weapon type
	float scaleSize = 0.;
	float nearFactor = 0.;

	// Choose the condition based on the weapon type or fire rate. Basically, what we need to achieve is that
	// when the weapon is a single shot type (i.e: shotgun, pistol) we must play the animation only once. 
	// Since we do not have any way to know this, we need to check if the weapon is a single shoot type, 
	// and then if so, check if a shot has already been fired. Thats because the shooting is done by 
	// the idle functions and keep checking if the button has been released or not.
	// Otherwise we use the current fire rate of the weapon to display correctly the shooting animation.
	bool gunShotCondition = -1;
	TYPE curWeaponType = this->player->inventory.weapons.at(this->player->currentweapon).type;
	if (curWeaponType == TYPE::PISTOL || curWeaponType == TYPE::SHOTGUN) {
		gunShotCondition = shotFired;
		scaleSize = 0.1;
		nearFactor = -0.03;

	}
	else {
		gunShotCondition = (bool)currate;
		scaleSize = 0.06;
	}

	// If the player is shooting, have ammo and match the current fire rate and the appropriate gunshot condition
	if (this->player->shootstate && curAmmo > 0 && !player->aimtransition && !gunShotCondition) {

		// Bind texture and enable it
		glBindTexture(GL_TEXTURE_2D, this->gunshot);
		glEnable(GL_TEXTURE_2D);

		// Push a new matrix in the stack for reset the transormation matrix 
		glPushMatrix();
		glLoadIdentity();

		// Load values 
		float aimx = this->player->inventory.weapons.at(this->player->currentweapon).aimx;
		float aimz = this->player->inventory.weapons.at(this->player->currentweapon).aimz;

		// Hard coded values for the animation
		float randscale = (float(rand()) / float((RAND_MAX)));
		float randangle = rand() % 360;
		glTranslatef(0.07 - aimx / 1.7, -0.045, -0.27 + nearFactor + aimz);
		glRotatef(randangle, 0.0, 0.0, 1.0);
		glScalef(scaleSize * randscale, scaleSize * randscale, scaleSize * randscale);
		//TODO: ROTATE THE TEXTURE WITH RANDOM VALUES!

		// Begin the drawing
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0, 1.0, -1.0);

		// End qauds, pop matrix and disable texturing
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}


}

// Simple function that draws a crosshair in the middle of the screen
void Drawer::draw_crosshair(void) {

	glDisable(GL_TEXTURE_2D);

	// Scale factor and resizing for the crosshair if the player is shooting
	float scaleFactor = this->player->shootstate ? 1.0f : 0.5f;
	float crosshairFactor = this->player->shootstate ? 0.01 : 0.0f;

	// Draw indipendent lines in the middle of the screen: first one is the classical 
	// crosshair 
	glColor3f(1.0, 0.0, 0.0);

	glPushMatrix();
	glLoadIdentity();
	glLineWidth(5.0*scaleFactor/6);
	glBegin(GL_LINES);
	glVertex3f(-0.020+crosshairFactor, 0.0, -1.0);
	glVertex3f(-0.005, 0.0, -1.0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.005, 0.0, -1.0);
	glVertex3f(0.020-crosshairFactor, 0.0, -1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.020+crosshairFactor, -1.0);
	glVertex3f(0.0, -0.005, -1.0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.005, -1.0);
	glVertex3f(0.0, 0.020-crosshairFactor, -1.0);
	glEnd();
	glPopMatrix();

	// Second crosshair rotated by 45 degree and enlarged when a player shoot
	glPushMatrix();
	glLoadIdentity();
	glLineWidth(4.0f * scaleFactor);
	glRotatef(45.0f, 0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(-0.015-crosshairFactor, 0.0, -1.0);
	glVertex3f(-0.005, 0.0, -1.0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.005, 0.0, -1.0);
	glVertex3f(0.015+crosshairFactor, 0.0, -1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, -0.015-crosshairFactor, -1.0);
	glVertex3f(0.0, -0.005, -1.0);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.005, -1.0);
	glVertex3f(0.0, 0.015+crosshairFactor, -1.0);
	glEnd();
	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
}


// Function that will draw a simple ray representing the shoot trajectory of the weapon
void Drawer::draw_shoot(void) {

	// Fetching the ammo of the current weapon of the player and the tick state and the shot fired 
	int curAmmo = this->player->inventory.weapons.at(this->player->currentweapon).ammo;
	int currate = this->player->tick % int(round((60 / this->player->inventory.weapons.at(this->player->currentweapon).firerate)));
	bool shotFired = this->player->inventory.weapons.at(this->player->currentweapon).singleShotWeaponFired;

	// Choose the condition based on the weapon type or fire rate. Basically, what we need to achieve is that
	// when the weapon is a single shot type (i.e: shotgun) we must play the animation only once. 
	// Since we do not have any way to know this, we need to check if the weapon is a single shoot type, 
	// and then if so, check if a shot has already been fired. Thats because the shooting is done by 
	// the idle functions and keep checking if the button has been released or not.
	// Otherwise we use the current fire rate of the weapon to display correctly the shooting animation.
	bool gunShotCondition = -1;
	TYPE curWeaponType = this->player->inventory.weapons.at(this->player->currentweapon).type;
	if (curWeaponType == TYPE::PISTOL || curWeaponType == TYPE::SHOTGUN) {
		gunShotCondition = shotFired;
	}
	else {
		gunShotCondition = (bool)currate;
	}

	// If the player is shooting and we got ammo (and we're into the tick state representing the fire rate of the cur weapon)
	if (this->player->shootstate && this->player->inventory.weapons.at(this->player->currentweapon).ammo > 0 && !player->aimtransition && !gunShotCondition) {

		// TODO: implement a on/off for the ray to be displayer 

			// Set the linewidth 
			if(this->player->inventory.weapons.at(this->player->currentweapon).type == TYPE::AUTOMATIC) glLineWidth(7.0f);
			else glLineWidth(12.0f);

			// Resetting the transformation stack and drawing a ray that starts from the center 
			// and goes to the tip of the weapon to represent a ray 
			glPushMatrix();
			glLoadIdentity();
			
			glPushAttrib(GL_COLOR_BUFFER_BIT);
			glColor3f(1.0f, 0.8f, 0.0f);
			glPopAttrib();

			// Rotation angle based on the view state 
			float rayangle = this->player->aimstate ? 57.0f : 0.0f;
			glRotatef(-33 - rayangle, 0.0, 0, 1.0);

			glBegin(GL_LINES);
			glVertex3f(0.3, 0.0, -1.0);
			glVertex3f(0, 0.0, -1.0);
			glEnd();
			glPopMatrix();

			// Reset linewitdth
			glLineWidth(1.0f);

	}

}

// Function that draws healthbox if presents 
void Drawer::draw_healthbox(void) {

	// If any healthbox
	if (this->consumablesManager->healthboxes.size() > 0) {

		// For every consumable still present in the world
		for (int i = 0; i < this->consumablesManager->healthboxes.size(); i++) {

			// Floating boolean reprsenting the need of an object to float
			bool floating = this->consumablesManager->healthboxes.at(i).floating;

			// To perform an animation where the consumables float in the air and "Pulsate"
			// (increase/and decrease its size) we must do it in a for loop using a "reached" 
			// bool flagg to understand when going back to the animation loop decreasing/increasing
			// size and y position
			// If floating, that means we need to move the object upward

			if (floating) {
				if (this->consumablesManager->healthboxes.at(i).y <= 1.5) {

					// increaese y pos and increase size a bit
					this->consumablesManager->healthboxes.at(i).y += 0.01;
					this->consumablesManager->healthboxes.at(i).scale += 0.0005;

					// if reached the maximum y position, set the floating state to false
					if (this->consumablesManager->healthboxes.at(i).y >= 1.5) this->consumablesManager->healthboxes.at(i).floating = false;

				}
			}
			else {
				
				// Else, we need to move the object downard
				if (this->consumablesManager->healthboxes.at(i).y >= 1) {
		
					// Decrease y pos and size a bit
					this->consumablesManager->healthboxes.at(i).y -= 0.01;
					this->consumablesManager->healthboxes.at(i).scale -= 0.0005;

					// if reached the minimum y position, set the floating state to false
					if (this->consumablesManager->healthboxes.at(i).y <= 1) this->consumablesManager->healthboxes.at(i).floating = true;

				}

			}

			// Declaring variables to ease us the code reading (avoiding infinite chain call to reach the desired
			// var)
			float health_posx = this->consumablesManager->healthboxes.at(i).x;
			float health_posy = this->consumablesManager->healthboxes.at(i).y;
			float health_posz = this->consumablesManager->healthboxes.at(i).z;
			float health_angle = this->consumablesManager->healthboxes.at(i).angle;
			float health_scale = this->consumablesManager->healthboxes.at(i).scale;

			// Push the transform matrix onto the stack and use the current healthbox 
			// position/angle/scale 
			glPushMatrix();
			glTranslatef(health_posx, 1, health_posz);
			glScalef(health_scale, health_scale, health_scale);
			glRotatef(health_angle, 0.0, 0, 1);

			// Render the model at 0 (healthbox model)
			this->consumablesManager->Templates.at(0)->draw();

			// Pop from the stack
			glPopMatrix();

		}

	}


}


// Function that draws ammobox if presents
void Drawer::draw_ammobox(void) {

	// Do the same for 
	if (this->consumablesManager->ammoboxes.size() > 0) {

		// For every consumable still present in the world
		for (int i = 0; i < this->consumablesManager->ammoboxes.size(); i++) {


			// Floating boolean reprsenting the need of an object to float
			bool floating = this->consumablesManager->ammoboxes.at(i).floating;

			// If the object has to be animated for the floating animation
			if (floating) {
				if (this->consumablesManager->ammoboxes.at(i).y <= 1.5) {

					// increaese y pos and increase size a bit
					this->consumablesManager->ammoboxes.at(i).y += 0.01;
					this->consumablesManager->ammoboxes.at(i).scale += 0.003;

					// if reached the maximum y position, set the floating state to false
					if (this->consumablesManager->ammoboxes.at(i).y >= 1.5) this->consumablesManager->ammoboxes.at(i).floating = false;

				}
			}
			else {

				// Else, we need to move the object downard
				if (this->consumablesManager->ammoboxes.at(i).y >= 1) {
		
					// Decrease y pos and size a bit
					this->consumablesManager->ammoboxes.at(i).y -= 0.01;
					this->consumablesManager->ammoboxes.at(i).scale -= 0.003;

					// if reached the minimum y position, set the floating state to false
					if (this->consumablesManager->ammoboxes.at(i).y <= 1) this->consumablesManager->ammoboxes.at(i).floating = true;

				}

			}


			float ammo_posx = this->consumablesManager->ammoboxes.at(i).x;
			float ammo_posy = this->consumablesManager->ammoboxes.at(i).y;
			float ammo_posz = this->consumablesManager->ammoboxes.at(i).z;
			float ammo_angle = this->consumablesManager->ammoboxes.at(i).angle;
			float ammo_scale = this->consumablesManager->ammoboxes.at(i).scale;






			// Push the transform matrix onto the stack and use the current healthbox 
			// position/angle/scale 
			glPushMatrix();
			glTranslatef(ammo_posx, 1, ammo_posz);
			glScalef(ammo_scale, ammo_scale, ammo_scale);
			glRotatef(ammo_angle, 0.0, 0, 1);

			// Render the model at 0 (ammobox model)
			this->consumablesManager->Templates.at(1)->draw();

			// Pop from the stack
			glPopMatrix();

		}

	}

}