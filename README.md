# Path-Planner-for-Phoenix-environment
## 1) Installation: 
1. Clone the master branch of the current repository, for SSH:\
	`git clone git@github.com:ostapvad/Path-Planner-for-Phoenix-environment.git` 
2. Create ROS workspace, choose any location and folder name(for example Gazebo_ws), then\
	`mkdir Gazebo_ws && mkdir Gazebo_ws`\
	`cd Gazebo_ws && catkin_make` 
3. Copy the folder consisting a Gazebo path plugin(gazebo_path_plugin) into your created workspace. You can do it manually or paste the following comands\
	 `cd Path-Planner-for-Phoenix-environment-master`\
	 `cp -a gazebo_path_plugin/ PathToYourWorkspace/Gazebo_ws/src`
## 2) Setting paths
1. Go to your home folder(/home/user_name),find .bashrc file, you've two options:
	* Manual: show all hidden files with the command
	`ctrl+h` and  open this file
	* Using terminal:
	`gedit .bashrc`
2. Insert the following commands in the end of your .bashrc file(you can open Info/my_bashrc to see the reference):
	* after\
	  `source PathWhereROSisInstalled\distribution\setup.bash`
	* change the path to your workspace, where you cloned the plugin(Gazebo_ws in our case)\
	`source /SomePath/Gazebo_ws/devel/setup.bash # Path to your workspace`\
	`export GAZEBO_PLUGIN_PATH=/SomePathGazebo_ws/devel/lib/ # Path to the same workspace for Gazebo plugins`\
	`export GAZEBO_MODEL_PATH=/SomePath/Gazebo_ws/src/gazebo_path_plugin/models:$GAZEBO_MODEL_PATH # Path to models for Gazebo simulator`	
3. I personally recommend to put the following commands after inserted in the previous part to see the pathes in a new terminal\
 	`echo $ROS_PACKAGE_PATH`\
	`echo $GAZEBO_PLUGIN_PATH`\
	`echo $GAZEBO_MODEL_PATH`
## 3) Compiling:
1. Before compiling(for Gazebo_ws in our case) go to\
 	`cd /SomePath/Gazebo_ws/src/gazebo_path_plugin/src/`
2. Open the spawn_world.hpp with any text editor and replace the following row\
	`std::string path_prefix =  "/home/ostapvad/Documents/Gazebo_ws/src/gazebo_path_plugin/models/";`\
with the path to your wokspace, something like this\
	`std::string path_prefix =  "/YourPath/YourWorkspace/src/gazebo_path_plugin/models/";`

## 4) Running
In the every new terminal put the command into the following order\
	`roscore` - to run ROS main node\
	`rosrun gazebo_ros gazebo -u`- to start Gazebo simulation node in pause mode\
	 a)To spawn the parking slots in a row \
	`rosrun gazebo_path_plugin builder` \
	 b)To build the parking box\
	`rosrun gazebo_path_plugin world_spawner` 
## 5) Shell files
## 6) Current results:
* *builder* node
![parking_slots](https://user-images.githubusercontent.com/49625282/114096627-45134b80-98bf-11eb-95b6-0d0e2625b7cf.jpg)
* *world_spawner* node 
![parking_box](https://user-images.githubusercontent.com/49625282/114097102-e1d5e900-98bf-11eb-9f66-143117fef583.jpg)


