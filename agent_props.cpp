#include "agent_props.hpp"

AgentProps::AgentProps() : id(0), generationIndex(0), numFoodsEaten(0) {}
AgentProps::AgentProps(unsigned int id, unsigned int generationIndex,
                       unsigned int numFoodsEaten)
    : id(id), generationIndex(generationIndex), numFoodsEaten(numFoodsEaten) {}