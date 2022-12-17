#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "GameEnums.h"

// Class that represent a wrapper for the score info of a given line in the csv file
class ScoreInfo {

public:

    // Attributes
    std::string name;
    GAMEMODE difficulty;
    int score;

    // Constructor/Destructor
    ScoreInfo(std::string name, GAMEMODE difficulty, int score) {
        this->name = name;
        this->difficulty = difficulty;
        this->score = score;
    }

    ScoreInfo() {}
    ~ScoreInfo() {}

    // Functions
    void setScoreInfo(std::string, int, GAMEMODE);


};

// Class that manages the scores into the game
class ScoreHandler {

public:

    // Attributes: 
    std::string scoreFilename;
    std::vector<ScoreInfo> scores;

    // Constructor/Destructor
    ScoreHandler(std::string scoreFilename = "resources/persistent/scoreboard.csv") {
        this->scoreFilename = scoreFilename;
    }
    ~ScoreHandler() {}

    // Functions
    void loadScores(void);
    void findPosition(ScoreInfo);
    void saveToFile(void);


private:

    // Functions
    std::vector<std::string> readFile(void);
    std::vector<std::string> scoresToString(void);
    void sortScores(void);

};
