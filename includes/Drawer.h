#pragma once
#include "TextureLoader.h"
#include "EnemyManager.h"
#include "ConsumablesManager.h"
#include "GameHandler.h"
#include "Player.h"
#include "Camera.h"
#include "Object.h"
#include "Weapon.h"
#include "RNG.h"
#include "Menu.h"
#include "GameEnums.h"


// Class that will take care of drawing element in the scene
class Drawer {

	public:
		Player* player;
		EnemyManager* enemyManager;
		ConsumablesManager* consumablesManager;
		GameHandler* gameHandler;
		Utils utils;
		Menu menu;
		RNG rng;
		int hudw, hudh;

		// Constructor: Parameters represents the scene limit (i.e: xlim and zlim = 100?
		// so the scene will range from -100 to 100, creating a square scene both for x and z.
		// To increase the scene size, just increase the x and z lim.
		// Tilesize represents the size of the ground tiles.
		// We also need the camera object instanciated in the main so we can use the camera informationn
		// about position to move certain objects.
		// Skybox dim reperesent the extension of the skybox.
		// OK is to avoid the overload with the default constructor.
		// Density and fogColor represent the fog 
		Drawer() {}
		Drawer(bool OK, float xlim = 150.0f, float zlim = 150.0f, float tilesize = 5.0f, float skyboxdim = 1.0f,
			float fogdensity = 0.008f, int hudw = 1920, int hudh = 1080)
		{

			this->fogdensity = fogdensity;
			this->xlim = xlim;
			this->zlim = zlim;
			this->tilesize = tilesize;	
			this->skyboxdim = skyboxdim;
			this->hudw = hudw;
			this->hudh = hudh;
			this->init();

		}
		
		// Destructor
		~Drawer() {}

		// Attributes
		float fogdensity;

		// Functions
		void drawScene(Object&);
		void drawTitleScreen(void);
		void drawDifficulty(void);
		void drawLoading(void);
		void drawAbout(void);
		void drawScores(void);
		void drawGameOver(void);
		void drawEnvironment(Object&);
		void drawInterface(void);
		void drawBlood(void);
		void drawObject(Object&, Vector3f, Vector3f, float, float);
		void drawEnemy(void);
		void drawConsumables(void);
		void drawUtil(void);
		void enableFog(fogtype);
		void clear(void);
		void setOrthographicProjection(void);
		void restorePerspectiveProjection(void);
		void updateTick();
		void loadGameResources(void);

	private:

		// Attributes
		TextureLoader texloader;
		Font fontmanager;
		GLuint skybox[6] = {};
		GLuint terrain;
		GLuint gunshot;
		GLuint interfaceHP;
		GLuint interfaceSkull;
		GLuint interfaceBullet;
		bool envfloat;
		float xlim;
		float zlim;
		float tilesize;
		float envy;
		float skyboxdim;
		int tick;
		int secondElapsed;
		
		// Init function
		void init();

		// Single context drawing function 
		void setup(void);
		void draw_terrain(void);
		void draw_skybox(void);
		void draw_cur_weapon(void);
		void draw_gunshot(void);
		void draw_crosshair(void);
		void draw_shoot(void);
		void draw_healthbox(void);
		void draw_ammobox(void);

			
};
