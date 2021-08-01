#include "additional_fcn.hpp"
#include "ros/ros.h"

// specify what to spawn
struct ParkingInfo{
  
    int parking_box_rows;
    float init_xy[2] = {0, 0};
    char parking_type; // 'C', 'D', 'F', '1', '2', '3', '4', '5'
};
std::vector<char> default_parking_types = {'C', 'D', 'F', '1', '2', '3', '4', '5'};

// MODIFY HERE!
// init parking = {row numbers, x, y, type from default_parking types}
std::vector<char> init_park_types = {'C'};
std::vector<float> init_pose_x = {50.0};
std::vector<float> init_pose_y = {40.0};
std::vector<float> init_row_nums = {10};



// template for the multidim tuple like ([...], [...], ...)
template<typename T>        // primary template
struct MultiTuple;          //leave it undefined

template<typename ...T>
struct MultiTuple<std::tuple<T...>>  //partial specialization
{
    using Type = std::tuple<std::vector<T>...>;
};




