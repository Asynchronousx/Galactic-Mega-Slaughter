#include "RNG.h"

///////////////////////
// PUBLIC FUNCTIONS //
//////////////////////

// Function that returns a random value between sxlim and dxlim
double RNG::getRng(float sxlim, float dxlim) {

    // Returning a random value between sxlim and dxlim
    return sxlim + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (dxlim - sxlim)));

}

// Function that return a random boolean between 0 and 1, based on a random number 
// generation and the belongining of a given interval of that random number 
bool RNG::getRngBool(float sxlim, float dxlim) {

    // Get a random number
    double rn = this->getRng(sxlim, dxlim);

    // Now check if that number is inside of a given interval betwen sxlim/dxlim: 
    // Ie, the random number is 4.5 and the interal is between 4 and 6: it belongs to it.
    // The functions need a value (rn) a centroid (sxlim+dxlim/2) and an offset.
    // The offset is simply calculated by taking the 1/5 part of the dxlim-sxlim: i.e, the 
    // sx lim is 0 and the dx lim is 10. So 1/5*(10-0) = 0.2*10 = 2. 
    // Again, sx lim is 0 and dx lim is 20, so 1/5*(20-0) = 0.2*20 = 4.
    // But what if we'd like to scale between ranges? Then sx lim = 10 and dx lim = 20. 
    // So we have 1/5*(20-10) = 1/5*10 = 2, again. 
    // Vary the standard luck (1/5) with other numbers to increase/decrease the chance.
    float centroid = (sxlim + dxlim) / 2;
    float offset = this->luck*(dxlim - sxlim);
    bool logicrng = this->utils.inRange(rn, centroid, offset);
    
    // Return the boolean generated
    return logicrng;


}


// Function that set the luck of the rng
void RNG::setLuck(float luck) {

    this->luck = luck;

}