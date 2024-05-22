#include "boid_props.hpp"

BoidProps::BoidProps() : id(0), generationIndex(0), numFoodsEaten(0) {}
BoidProps::BoidProps(unsigned int id, unsigned int generationIndex,
                     unsigned int numFoodsEaten)
    : id(id), generationIndex(generationIndex), numFoodsEaten(numFoodsEaten) {}