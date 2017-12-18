#include "food.h"

const GameObjectCategory Food::s_category{"FOOD"};

Food::Food(double latitude, double longitude)
    : GameObject{Food::category(), latitude, longitude, 0} {}
