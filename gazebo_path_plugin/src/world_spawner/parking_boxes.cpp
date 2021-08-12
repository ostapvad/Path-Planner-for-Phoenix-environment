// Parsing the parking box(not world_spawner node) parameters

#include "parking_slots.hpp"

#include "geometry_msgs/Pose.h"
#include <vector>


namespace ParkingBoxes {
    class Box{
        public:
            std::string name;
            struct Positon 
            {
                float x, y;
            }pose;
            std::vector<float> rotations; 
            int type_id, sim_id;  // type from all types, id given from simulation
            int slots_number = 0; // number of current slot
        
            std::vector<std::string> names = {"Left closed with one column", "Right closed with one column", "Left closed with two columns", "Right closed with two colums", "Opened with two columns"}; // {1, 2, 3, 4, 5}
            std::vector<Objects::ParkingSlot*> slots;
        Box(int type_id, float Position[2], int sim_id){
            try {
                this->name = names[type_id-1];
            }
            catch(const std::out_of_range& oor){
                    std::cout<<"Ids of boxes must be in range of {1, 2, 3, 4, 5}. Error!" << std::endl;
                    exit(1);
            }
            
            this->type_id = type_id;
 
            set_parameters(Position, sim_id);

        }
        // define the parking box with the rotations 
        void set_parameters(float Position[2], int sim_id){
                this->pose.x = Position[0];
                this->pose.y = Position[1];
                this->sim_id = sim_id;
                switch (this->type_id)
                {
                case 1:
                    this->rotations.push_back(90);
                    break;
                case 2:
                    this->rotations.push_back(-90);
                    break;
                case 3:
                    this->rotations.push_back(90);
                    this->rotations.push_back(90);
                    break;
                case 4:
                    this->rotations.push_back(-90);
                    this->rotations.push_back(-90);
                    break;
                case 5:
                    this->rotations.push_back(-90);
                    this->rotations.push_back(90);
                    break;
                }
        }
        
        //Show the parameters of the parking box
        void ShowParameters(){
            std:: cout << "-----------------------------------------------------\nName: " << this->name <<std::endl;
            printf("Type ID: %d\n", this->type_id);
            printf("Simulation ID: %d\n", this->sim_id);
            printf("Position: (x,y) = (%f,%f)\n", this->pose.x, this->pose.y);
            printf("Slots number: %d\n", this->slots_number);
            std::cout<<"-----------------------------------------------------\n";
        }
        
        // append the parking slot to the array
        void CreateSlot(int row, int col){
            Objects::ParkingSlot *p = new Objects::ParkingSlot;
            p->place.row = row;
            p->place.col = col;
            p->id = this->slots_number;
            slots.push_back(p);
            this->slots_number ++;
        }
        
        // check if there is a car on the parking slot
        bool is_full(int row, int col){
                for (int i = 0; i < this->slots_number; i++){
                    if(slots[i]->place.row == row && slots[i]->place.col == col){
                        return slots[i]->with_car;
                    }

                }
                return false;
            
        }
        
        // update the car status on the parking slot
        void UpdatePlace(int row, int col, bool value){
             for (int i = 0; i < this->slots_number; i++){
                    if(slots[i]->place.row == row && slots[i]->place.col == col){
                        slots[i]->with_car = value;
                        break;
                    }

                }


        }

    

    };
    


}

