#include "GameHandler.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that goes to the next selection in the main screen
void GameHandler::menuNextSelection(void) {

	// Switch type of entry based on the state
	if (this->state == STATE::TITLE) {
		if (this->menuEntry < 3) this->menuEntry++;
		else this->menuEntry = 0;
	}
	else if (this->state == STATE::DIFFICULTY) {
		if (this->difficultyEntry < 3) this->difficultyEntry++;
		else this->difficultyEntry = 0;
	}

	// Play selection sound
	this->playSelectionSound();

}

// Function that goes to the prev selection in the main screen
void GameHandler::menuPrevSelection(void) {

	// Switch type of entry based on the state
	if (this->state == STATE::TITLE) {
		if (this->menuEntry > 0) this->menuEntry--;
		else this->menuEntry = 3;
	}
	else if (this->state == STATE::DIFFICULTY) {
		if (this->difficultyEntry > 0) this->difficultyEntry--;
		else this->difficultyEntry = 3;
	}

	// Play selection sound
	this->playSelectionSound();

}

// Function that based on the current state/entry will load a new state
void GameHandler::enterNewState(void) {

	// Based on the current state
	switch (this->state) {

		case STATE::TITLE:

			// Play enter sound 
			this->playEnterSound();

			// Switch menu entyr selection and change state based on the type of entry
			switch (this->menuEntry) {

				case 0:
					this->state = STATE::DIFFICULTY;
					break;

				case 1:
					this->state = STATE::SCORES;
					break;

				case 2:
					this->state = STATE::ABOUT;
					break;

				case 3:
					this->state = STATE::EXIT;
					return;
					
			}


			break;

		case STATE::DIFFICULTY:

			// Play enter sound 
			this->playEnterSound();

			// Switch difficulty entry selection
			switch (this->difficultyEntry) {

				case 0:
					this->difficulty = GAMEMODE::EASY;
					break;

				case 1:
					this->difficulty = GAMEMODE::NORMAL;
					break;

				case 2:
					this->difficulty = GAMEMODE::HARD;
					break;

				case 3:
					this->difficulty = GAMEMODE::MADNESS;
					break;

			}

			// once the difficulty has been choosen, change the state in loading 
			this->state = STATE::LOADING;

			// Stop menu sounds
			this->mBackground->stop();
			this->mBackground->drop();
			break;

		case STATE::SCORES:

			// If the case is scores, we need to go back to the main menu when pressing enter
			this->state = STATE::TITLE;
			break;

		case STATE::ABOUT:

			// same as above, but for about
			this->state = STATE::TITLE;
			break;

		case STATE::GAMEOVER:

			// Play enter sound 
			this->playEnterSound();

			// same as above, but for game over
			this->state = STATE::TITLE;

			// Stop game sounds 
			this->mGame->stop();
			this->mGame->drop();

			// Play game sounds
			this->playTitleTheme();

			break;


	}


}

// Restoring the main title screen state
void GameHandler::restoreTitleState(void) {

	this->state = STATE::TITLE;

}

// Function that loads the scoreboard from the apposit object 
void GameHandler::loadScoreboard(void) {

	this->scoreHandler.loadScores();

}

// Function that saves the scoreboard on exit 
void GameHandler::saveScoreboard(void) {

	this->scoreHandler.saveToFile();

}

// Function that insert a new score into the scoreboard 
void GameHandler::insertNewScore(ScoreInfo newScore) {

	this->scoreHandler.findPosition(newScore);

}

// Function that return a string containing the difficulty name
std::string GameHandler::getDifficultyString(GAMEMODE mode) {

	// String to be filled 
	std::string diff;

		// Switching casese
	switch (mode) {

		case GAMEMODE::EASY:
			diff = "EASY";
			break;

		case GAMEMODE::NORMAL:
			diff = "NORMAL";
			break;

		case GAMEMODE::HARD:
			diff = "HARD";
			break;

		case GAMEMODE::MADNESS:
			diff = "MADNESS";
			break;

	}

	return diff;

}

void GameHandler::playTitleTheme(void) {

	// Play the title theme using the sound engine, set the variable to true 
	// relative to the theme playing
	if (!this->titleThemePlaying) {

		this->mBackground = this->engine->play2D("resources/audio/title/theme.mp3", true, false, true);
		this->titleThemePlaying = true;
		this->gameThemePlaying = false;

	}

}

void GameHandler::playGameOverTheme(void) {

	this->engine->play2D("resources/audio/gameover/gameover.mp3", false);

}

void GameHandler::playGameTheme(void) {

	// Play the title game using the sound engine, set the variable to true 
	// relative to the game playing
	if (!this->gameThemePlaying) {

		this->mGame = this->engine->play2D("resources/audio/game/cyber.mp3", true, false, true);
		this->engine->play2D("resources/audio/game/enemydetected.mp3", false, false, true);
		this->titleThemePlaying = true;
		this->titleThemePlaying = false;

	}

}

// Function that plays keyboard sound
void GameHandler::playKeySound(void) {

	// Play the title game using the sound engine, set the variable to true 
	// relative to the game playing
	this->engine->play2D("resources/audio/effects/keyboard.mp3", false);

}

// Function that plays menu switch soundd
void GameHandler::playEnterSound(void) {

	// Play enter new state sound 
	this->engine->play2D("resources/audio/effects/click.mp3", false);

}

// Function that play confirmation sounds
void GameHandler::playSelectionSound(void) {

	// Play enter new state sound 
	this->engine->play2D("resources/audio/effects/selection.mp3", false);

}

// Function that plays a generic audio given a source
void GameHandler::playGenericAudio(std::string source) {

	this->engine->play2D(source.c_str() , false);

}

// Simple init function
void GameHandler::init(void) {


	// Initially, set variables 
	this->menuEntry = 0;
	this->difficultyEntry = 0;
	this->state = STATE::TITLE;
	this->assetLoaded = false;
	this->engine = irrklang::createIrrKlangDevice();

}