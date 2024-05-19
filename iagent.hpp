#pragma once

class IAgent {
public:
  virtual ~IAgent() {}
  virtual unsigned int getId() const = 0;
  virtual void step(float timeDelta) = 0;
};