#pragma once

#include "food_source.hpp"

class IFoodSourceListener {
public:
	virtual ~IFoodSourceListener() {}
	virtual void foodSourceCreated(FoodSource foodSource) = 0;
	virtual void foodSourceDestroyed(FoodSource foodSource) = 0;
};