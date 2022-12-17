#include "Enemy.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that will move at a given speed the enemy in the world to follow the player.
// Needs the actual coordinate of the player to move to it
void Enemy::move(float playerx, float playery, float playerz) {
	
	// The logic is simple: we want to calculate the difference between the actual XZ position 
	// of the player-enemy. Got that, we simply move the enemy towards that line
	// Getting the difference points:
	float dirx = playerx - this->posx;
	float diry = playery - this->posy;
	float dirz = playerz - this->posz;

	// Normalizing the resultant vector to have unit lenghts
	float hypo = sqrt(dirx * dirx + diry * diry + dirz * dirz);
	dirx /= hypo;
	diry /= hypo;
	dirz /= hypo;

	// We do update the angle of the enemy to face the player 
	this->dirangle = (atan2(-dirz, dirx) * 180 / 3.14159265) + 90;

	// Then we move to the the player with desired speed
	this->posx += dirx * this->speed;
	this->posy += diry * this->speed;
	this->posz += dirz * this->speed;

}

// Function thata draw a weapon calling the inhereited render method.
void Enemy::draw(void) {

	// Wrapper for rendering an object
	this->render();

}

// Function overload: we call the superclass load function and after that we set the bounding box 
// vertex in the apposit data structure 
void Enemy::loadAsset(Color4f colors, std::string path, std::string filename, std::string material_filename = "") {
	
	// Loading the model 
	this->load(colors, path, filename, material_filename);

	// Assigning the OBB vertices: 0 is x, 1 is y, 2 is z
	for (int i = 0; i < 3; i++) {
		this->minVertex.push_back(this->model.minimumVertex()[i]*this->scale);
		this->maxVertex.push_back(this->model.maximumVertex()[i]*this->scale);
	}

}

// Function that randomize the creation of the enemy including its position in the world, 
// speed, scale hp and power
void Enemy::randomize(GAMEMODE difficulty) {

	// Randomizing speed, scale and position
	this->posx = rand() % 200 + (-100);
	this->posy = rand() % 25 + 3;
	this->posz = rand() % 200 + (-100);
	this->speed = this->rng.getRng(0.01, 0.13);
	this->scale = this->rng.getRng(0.05, 1.);

	// Assigning HP and POWER based on the difficulty 
	float multiplier = 0;

	switch (difficulty) {

	case GAMEMODE::EASY:
		multiplier = 0.5;
		break;


	case GAMEMODE::NORMAL:
		multiplier = 1.0;
		break;

	case GAMEMODE::HARD:
		multiplier = 2.0;
		break;

	case GAMEMODE::MADNESS:
		multiplier = 4.0;
		break;

	}

	this->hp = (rand() % 100 + 100) * multiplier;
	this->power = 1 * multiplier;


}

// Function that takes the damage to the enemy 
void Enemy::takeDamage(int dmg, int soundTick) {

	// Scale hp
	this->hp -= dmg;

	// Play hit sound once every 1/6 of sec if being hit
	if (!(soundTick % 10)) this->hitSound();


}

// Function that switch the types and return an id 
int Enemy::getID(void) {

	return (int)this->identifier;

}

// Functions that plays some audio on specific situations
void Enemy::hoverSound(void) {

	this->engine->play2D("resources/audio/enemy/hoover.mp3", false);

}

// Functions that plays some audio on specific situations
void Enemy::hitSound(void) {

	// Play 2 different sounds based on the rng 
	if (this->rng.getRngBool(0, 10)) this->engine->play2D("resources/audio/enemy/hit_1.mp3", false);
	else  this->engine->play2D("resources/audio/enemy/hit_2.mp3", false);


}

// Functions that plays some audio on specific situations
void Enemy::deathSound(void) {

	// Play 2 different sounds based on the rng 
	if (this->rng.getRngBool(0, 10))  this->engine->play2D("resources/audio/enemy/death_1.mp3", false);
	else  this->engine->play2D("resources/audio/enemy/death_2.mp3", false);

}

///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

// Init of attributes
void Enemy::init(IDENTIFIER identifier, GAMEMODE difficulty) {

	// TMP: THIS SHOOULD GO IN ENGINE 
	srand(time(NULL));

	// Initializing enemy values and attributes 
	this->posx = 0;
	this->posy = 0;
	this->posz = 0;

	// Angle of the enemy on the floor facinng the player 
	this->dirangle = 0;
	
	// Speed of the enemy 
	this->speed = 0;

	// Scale of the enemy
	this->scale = 0;

	// Hp and power 
	this->hp = 0;
	this->power = 0;

	// Status 
	this->alive = true;
	this->beingShot = false;

	// Assigning his name 
	this->identifier = identifier;

	// Randomize values 
	this->randomize(difficulty);

	// Init sound engine
	this->engine = irrklang::createIrrKlangDevice();

}

