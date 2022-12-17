#include "ScoreHandler.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Simple function that set the argument of the scoreinfo object
void ScoreInfo::setScoreInfo(std::string name, int score, GAMEMODE difficulty) {

    this->name = name;
    this->score = score;
    this->difficulty = difficulty;

}

// Function that given an unparsed vector of csv line will parse it into ScoreInfo objects
void ScoreHandler::loadScores(void)
{

    // Load an unparsed vector of csv strings 
    std::vector<std::string> unparsedScores = this->readFile();

    // Now, we have a vector of string composed in this way: NAME;DIFFICULTY;SCORE. 
    // We need, for each string, to extract the three elements and put it inside a ScoreInfo obj.
    // We'll use a stringstream on each string of the unparsedScore to extract the values. 
    for (int i = 0; i < unparsedScores.size(); i++) {

        // Extracting the stringstream at i
        std::stringstream ss(unparsedScores.at(i));

        // Creating a vector of strings that will contains the 3 occurrences
        std::vector<std::string> values;

        // While the stream contains something
        while (ss.good()) {

            // Get the first token before the delimiter
            std::string substr;
            getline(ss, substr, ';');

            // Pushing back the current value 
            values.push_back(substr);

        }

        // Once Here, we got into our values vector a full parsed line of the scoreboard in 
        // format NAME, DIFF, SCORE at their relative position (0,1,2). Using that knowledge, 
        // We do create a score object and assigning the relatives fields. 
        this->scores.push_back(ScoreInfo(values.at(0), (GAMEMODE)std::stoi(values.at(1)), std::stoi(values.at(2))));

    }

    // Once we loaded our scoreboard, we then sort it in descent order by the score value.
    // In this way, at the first place we'll have the biggest score, and ad the end the lower. 
    // Sorting the file will result in easier handling of the scores when a new player plays a game
    // and perform a new score, and when writing again to the file.
    // We sort the score vector using some lambda function.
    this->sortScores();

}

// Function that given a new score object will find its position into the score array 
void ScoreHandler::findPosition(ScoreInfo curScore) {

    // Since the score vector is already sorted, this is basically done by a linear search 
    // through the score vector. We do compare at each index the score of the ith element with the
    // new one, and insert the score element into the right position.
    // If the size of the scores are major than zero 
    if (this->scores.size() > 0) {

        // Position index is -1 at start, so we can check at the end of iteration if we found a new place
        // to insert our index
        int positionIndex = -1;

        // For all the score in scores
        for (int i = 0; i < this->scores.size(); i++) {

            // If the new player score is major than the ith score (sorted in descent order)
            if (curScore.score >= this->scores.at(i).score) {

                // Update the position and continue to iterate to find the best possible 
                // score to update
                positionIndex = i;

            }

        }

        // Insert the new score at the given position if found an insertion place
        if (positionIndex != -1) this->scores.insert(this->scores.begin() + positionIndex, curScore);

    }
    else {

        // Else, the scoreboard is empty: just push it into the first place
        this->scores.push_back(curScore);

    }

    // Now, check if the score vector is major than 10. That's because we want to log AT MAX 10 scores 
    // into our file/score tracker. The score that have a position into the score vector > 10 will be popped.
    // We also do not need a for because with this check the score vector will never have a dimension > 10.
    if (this->scores.size() > 10) {

        this->scores.pop_back();

    }

    // Sorting again the scores 
    this->sortScores();

}

// Function that will save the score vector to a file on exit
void ScoreHandler::saveToFile(void) {

    // We first dump our score vector to a string vector containing csv strings 
    std::vector<std::string> csvString = this->scoresToString();

    // Now, we overwrite the previous file with those new scores
    std::fstream file;

    // Open the file in out mode to overwrite exsisting content
    file.open(this->scoreFilename, std::ios::out);

    for (auto s : csvString) {
        file << s + "\n";
    }

}


///////////////////////
// PRIVATE FUNCTIONS //
//////////////////////

// Function that reads a CSV file and copy the lines into a data structure to be parsed
std::vector<std::string> ScoreHandler::readFile(void) {

    // Create a vector of <string> to store the unparsed result in format val;val;val (name, difficultu, score)
    std::vector<std::string> unparsedResult;

    // Create an input filestream
    std::ifstream myFile(this->scoreFilename);

    // Make sure the file is open
    if (!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    std::string line, colname;

    // If the file is good
    if (myFile.good())
    {

        // Read data, line by line
        while (std::getline(myFile, line))
        {
            // Create a stringstream of the current line
            std::stringstream ss(line);

            // Push back the line into the unparsed string vector 
            unparsedResult.push_back(line);

        }

    }

    // Close file
    myFile.close();

    // Return the unparsed results
    return unparsedResult;

}

// Function that perform the inverse operation of the loading scores into scores object
std::vector<std::string> ScoreHandler::scoresToString(void) {

    // We basically need to invert the operation and transform the score objects into 
    // CSV strings again. 
    // Declaring a string vector to store csv string;
    std::vector<std::string> csvScores;

    // We sort the scores 
    this->sortScores();

    // For the size of the score vector 
    for (int i = 0; i < this->scores.size(); i++) {

        std::string name = this->scores.at(i).name;
        std::string difficulty = std::to_string((int)this->scores.at(i).difficulty);
        std::string score = std::to_string(this->scores.at(i).score);

        // Push the combination into the vector 
        csvScores.push_back(name + ";" + difficulty + ";" + score);

    }

    // Return the string vector 
    return csvScores;

}


// Function that sorts the scores vector 
void ScoreHandler::sortScores(void) {

    // Using lambda functions, we sort the score vector based in a descend order on the score value
    std::sort(this->scores.begin(), this->scores.end(), [](const ScoreInfo& lhs, const ScoreInfo& rhs) {
        return lhs.score > rhs.score;
        }
    );

}