// Some additional functions 
#include <fstream>
#include "geometry_msgs/Quaternion.h"
#include <cmath>



/*Reads the file content in the string */
std::string readFileIntoString2(const std::string & str_path) { 
    auto ss = std::ostringstream{};
    //std::filesystem::path;

    std::ifstream input_file(str_path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << str_path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    return ss.str();
}

/*Calculates the rotation of object around the z axis, more quicker than default ROS */
void RotZToQuat(geometry_msgs::Quaternion & new_pose, float angle_deg){ 

 
  new_pose.w = sqrt(2 + 2*cos(angle_deg*M_PI/180.0))/2;
  new_pose.x = new_pose.y = 0;
  new_pose.z = sin(angle_deg* M_PI/180.0)/(2*new_pose.w);

}

/*Removes the spaces betweeen comas*/
std::string remove_spaces(std::string sub_word){
    int start_slice = 0;
    int end_slice = sub_word.size()-1;
    while(sub_word[end_slice]==' '){
            end_slice--;}
    while(sub_word[start_slice]==' '){
            start_slice++;}
    return  sub_word.substr(start_slice, end_slice -start_slice +1);

}

/*Parses python list or a single variable format to array of strings*/
std::vector<std::string> parsePythonList(std::string line_list){
    std::vector<std::string> words;
    int begin_ind = 0;
    int end_ind = 1;
   while((end_ind = line_list.find(",", begin_ind+1))!=-1){
        words.push_back(remove_spaces(line_list.substr(begin_ind+1, end_ind -begin_ind-1)));
        begin_ind = end_ind;

    }
    if (begin_ind==0){
        if((end_ind = line_list.find("]", begin_ind+1))!=-1) words.push_back(remove_spaces(line_list.substr(begin_ind+1, end_ind -begin_ind-1)));
        else  words.push_back(remove_spaces(line_list.substr(0, line_list.size())));
    }
    else {
        end_ind = line_list.find("]", begin_ind+1);
        words.push_back(remove_spaces(line_list.substr(begin_ind+1, end_ind -begin_ind-1)));
    }
    
    return words;

}

/*Experimental function that creates the borders row. You can copy its content to model.sdf of borders folder*/
std::string update_borders(){
    const int N = 8;
    std::vector<std::string> tags = {"include> ", "uri> ", "name> ", "pose> ", "model> ", "sdf> "};
    std::vector<std::string> model_names ={"model://orange_barrier", "model://white_barrier"};   
    std::string paragraph_string = "<?xml version='1.0'?> \n <sdf version='1.6'>\n <model name='path_borders'>\n";
    
    float position[3] = {0, -1.5, 0};
    std::string pose = "";
    for (int i=0; i < N; i++){
        paragraph_string += " <" + tags[0] + "\n";
        paragraph_string += " <"+ tags[1] + model_names[(i%2)&1] + " </"+tags[1] + "\n";
        paragraph_string += " <" + tags[2] + "barr_"+std::to_string(i)+ "</" +tags[2] + "\n";
        pose = " " + std::to_string(position[0]) + " " + std::to_string(position[1]) + " " + " 0 0 0 1.57075";
        paragraph_string += " <" + tags[3] + pose + "</" + tags[3] + "\n";
        paragraph_string += " </" + tags[0] + "\n";
        position[0] += 1.75;
    }
    paragraph_string += " </" + tags[4] + "\n" + "</" + tags[5] +"\n";
    
    
    std::cout << paragraph_string;
    return paragraph_string;
}
