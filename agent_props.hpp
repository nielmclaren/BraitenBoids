#pragma once

#include <cereal/access.hpp>
#include <cereal/cereal.hpp>
#include <string>
#include <vector>

class AgentProps {
  friend class cereal::access;

  template <class Archive> void serialize(Archive &archive) {
    archive(CEREAL_NVP(id), CEREAL_NVP(generationIndex),
            CEREAL_NVP(numFoodsEaten), CEREAL_NVP(weights));
  }

public:
  unsigned int id;
  unsigned int generationIndex;
  unsigned int numFoodsEaten;

  std::vector<float> weights;

  AgentProps();
  AgentProps(unsigned int id, unsigned int generationIndex,
             unsigned int numFoodsEaten);
};
