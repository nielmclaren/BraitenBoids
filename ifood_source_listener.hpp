#pragma once

#include "food_source.hpp"

class IFoodSourceListener {
public:
	virtual ~IFoodSourceListener() {}
	virtual void foodSourceCreated(FoodSource* foodSource) = 0;
	virtual void foodSourceDeleted(FoodSource* foodSource) = 0;
};