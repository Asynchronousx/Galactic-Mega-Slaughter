#pragma once
#include "GameEnums.h"
#include "ScoreHandler.h"
#include "externals/IrrKlang/includes/irrKlang.h"

// Class that manages the menu entries and take track of current selections
class GameHandler {

	public:

		// Attributes
		STATE state;
		GAMEMODE difficulty;
		ScoreHandler scoreHandler;
		irrklang::ISoundEngine* engine;
		irrklang::ISound* mBackground;
		irrklang::ISound* mGame;
		int menuEntry;
		int difficultyEntry;
		bool assetLoaded;
		bool titleThemePlaying = false;
		bool gameThemePlaying = false;


		// Constructor/Destructor
		GameHandler() {
			this->init();
		}
		~GameHandler() {}

		// Functions 
		void menuNextSelection(void);
		void menuPrevSelection(void);
		void enterNewState(void);
		void restoreTitleState(void);
		void insertNewScore(ScoreInfo);
		void loadScoreboard(void);
		void saveScoreboard(void);
		void playTitleTheme(void);
		void playGameTheme(void);
		void playGameOverTheme(void);
		void playKeySound(void);
		void playSelectionSound(void);
		void playEnterSound();
		void playGenericAudio(std::string);
		std::string getDifficultyString(GAMEMODE);

	private:

		// Functions 
		void init(void);

};