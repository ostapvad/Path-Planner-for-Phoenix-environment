#include <iostream>

namespace Objects{

struct ParkingSlot // definition of the parking slot
{
    int id;
    struct Place
    {
        int row, col;
    } place;
    bool with_car = false; // defines, if the current parking slot contains the car
    
};
}
