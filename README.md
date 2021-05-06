# Path-Planner-for-Phoenix-environment
## 1) Installation: 
1. Create ROS workspace, choose any location and folder name(for example Gazebo_ws), then\
	`mkdir Gazebo_ws && mkdir Gazebo_ws`\
	`cd Gazebo_ws && catkin_make` 
2. Clone the master branch of the current repository into your created Gazebo_ws/src, for SSH:\
	`git clone git@github.com:ostapvad/Path-Planner-for-Phoenix-environment.git` 
## 2) Setting paths
You can skip this part, but I recommend you to set the parameters in your .bashrc file following these steps, otherwise you'll have to do it manually after openning a new terminal.
1. Go to your home folder(/home/user_name),find .bashrc file, you've two options:
	* Manual: show all hidden files with the command
	`ctrl+h` and  open this file
	* Using terminal:
	`gedit .bashrc`
2. Source your created workspace. Insert the following commands in the end of your .bashrc file(you can open Info/my_bashrc to see the reference):
	* after\
	  `source PathWhereROSisInstalled\distribution\setup.bash`
	* change the path to your workspace, where you cloned the plugin(Gazebo_ws in our case)\
	`source /SomePath/Gazebo_ws/devel/setup.bash # Path to your workspace`\
3. I personally recommend to put the following commands after inserted in the previous part to see the pathes in a new terminal\
 	`echo $ROS_PACKAGE_PATH`\
	`echo $GAZEBO_PLUGIN_PATH`\
	`echo $GAZEBO_MODEL_PATH`
## 3) Compiling:
1. Go back to the root of your ROS workspace(Gazebo_ws in our case)\
 	`cd /SomePath/Gazebo_ws/`
2. Compile your workspace with\
         `catkin_make`\
 New GAZEBO model and plugin path will be added automatically\
        `export GAZEBO_PLUGIN_PATH=/SomePathGazebo_ws/devel/lib/ # Path to the same workspace for Gazebo plugins`\
	`export GAZEBO_MODEL_PATH=/SomePath/Gazebo_ws/src/gazebo_path_plugin/models:$GAZEBO_MODEL_PATH # Path to models for Gazebo simulator`	
## 4) Running
1) In the every new terminal put the command into the following order\
	`roscore` - to run ROS main node\
	`rosrun gazebo_ros gazebo -u`- to start Gazebo simulation node in pause mode\
2) In your third terminal you can get the list of all avaliable services and you should see some with prefix `/gazebo/`. Then run the world spawner client with the default parameters: 
	`rosrun gazebo_path_plugin world_spawner`
3) Now you can change your world or save. **Attention!** All the changes you do with your world manually won't be monitored by the world spawner, it only helps you to spawn the specific type of the world.
4) Also if you have [steerbot package](https://github.com/peconver/skoda_simulation/tree/master/src/ackermannsteer/steerbot) in the same workspace as the current plugin, you can launch a robot to the created world with [launcher](gazebo_path_plugin/launch/spawn_steerbot.launch) by\
	`roslaunch steerbot spawn_steerbot.launch` 
## 5) Parameters:
1. All the parameters have the prefix **/world_spawner**. You can read about the parameters meaning in the [description](Other/INFO/SpawnerDescription.pdf).
2. You can set the required parameter with\
	`rosparam set /world_spawner/parameter_name`\
or load from .yaml file\
	`rosparam load pathToYamlFile.yaml`\
The example of the .yaml file for world spawner client is given in [world.yaml](/gazebo_path_plugin/config/world.yaml). Please, get acquainted with it.
3. After setting your parameters, you have to run the world spawner again and the parameters will be updated to yours. **Attention!** Also the client runs so that  firstly deletes all spawned models before, then takes the given parameters and spawns new world. Deleting all the models can take some amount of time, so I recommend you to abort `rosrun gazebo_ros gazebo -u` and run it again and then run world spawner client. It will delete only the single ground plane which want take much time.
4. Alternative way of running with the updated parameters, the easiest one. You can set the new parameters in [world.yaml](/gazebo_path_plugin/config/world.yaml), save it. If there has nothing been spawned yet(recommended), run the spawner with parameters from [world.yaml](/gazebo_path_plugin/config/world.yaml) using the [roslaunch file](/gazebo_path_plugin/launch/loadWorldParam.launch)\
	`roslaunch gazebo_path_plugin loadWorldParam.launch`\
Also you can update this [roslaunch file](/gazebo_path_plugin/launch/loadWorldParam.launch) to set the parameters from another .yaml, just replace the path
 `	<rosparam file="$(find gazebo_path_plugin)/config/world.yaml" />` to your new created .yaml file.
## 6) Current results:
1) Blok C with 10 rows in the each parking box, cars were spawned randomly on the parking slots
![default_gzclient_camera(1)-2021-04-14T20_50_12 611681](https://user-images.githubusercontent.com/49625282/114766896-42a16d80-9d67-11eb-83bd-d9ad24a2b903.jpg)
2) Full blok C with 10 rows and 140 cars on the parking slots
![default_gzclient_camera(1)-2021-05-05T21_59_35 282525](https://user-images.githubusercontent.com/49625282/117245072-7a697580-ae3a-11eb-977d-05a027f742a2.jpg)




