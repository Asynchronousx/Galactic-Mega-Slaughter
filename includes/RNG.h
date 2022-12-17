#pragma once
#include <random>
#include "Utils.h"

// Class that represent the random number generation from the monster drops in the world
class RNG {

    public: 

        // Attributes
        Utils utils;

        // Constructor/Destructor 
        RNG(float luck = 0.3) {
            this->luck = luck;
            srand(time(0));
        }

        ~RNG() {}

        // Functions 
        double getRng(float, float);
        bool getRngBool(float, float);
        void setLuck(float);


    private:

        // Attributes 
        float luck;

};
