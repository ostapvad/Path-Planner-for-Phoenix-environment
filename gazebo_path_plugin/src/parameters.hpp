#include <iostream>

namespace Objects{

struct ParkingBlock // definition of the parking block
{
    int id, num_slots; // id, number of slots in the each row
    std::string name; // name of the parking block
    struct ParkingSlot *slots; //
    
};
struct ParkingSlot // definition of the parking slot
{
    int id_slot, parent_id;
    std::string name;
    bool with_car = false; // defines, if the current parking slot contains the car
    
};
}
