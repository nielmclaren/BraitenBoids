#pragma once

#include "boid.hpp"

class IBoidListener {
public:
	virtual ~IBoidListener() {}
	virtual void boidCreated(Boid* boid) = 0;
	virtual void boidDeleted(Boid* boid) = 0;
};