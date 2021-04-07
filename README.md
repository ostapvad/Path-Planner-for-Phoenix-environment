# Path-Planner-for-Phoenix-environment
## 1) Installation: 
1. Clone the master branch of the current repository, for SSH:\
	`git clone git@github.com:ostapvad/Path-Planner-for-Phoenix-environment.git` 
2. Create ROS workspace, choose any location and folder name(for example Gazebo_ws), then\
	<p align="center">
	`mkdir Gazebo_ws && mkdir Gazebo_ws`\
	`cd Gazebo_ws && catkin_make`
 	</p>
3.
4. Copy the folder consisting a Gazebo path plugin into your 
5) Copy the folder gazebo_path_plugin to your workspace/src and catkin_make.
6) Update your .bashrc file, paste at the end, change the path to your workspace.
	source /home/ostapvad/Documents/Gazebo_ws/devel/setup.bash # Path to your workspace
	export GAZEBO_PLUGIN_PATH=~/Documents/Gazebo_ws/devel/lib/ # Path to the same workspace for Gazebo plugins
	export GAZEBO_MODEL_PATH=/home/ostapvad/Documents/Gazebo_ws/src/gazebo_path_plugin/models:$GAZEBO_MODEL_PATH
4) cd to Shell/, update the line in run_mypark.sh to your workspace
 	ws_path="/home/ostapvad/Documents/Gazebo_ws" # path to ROS workpace CHANGE TO YOUR CUSTOM
5) run with ./run_mypark.sh(for tmux users). Otherwise go to your worspace and run in separate terminal the commands:
	roscore
	rosrun gazebo_ros gazebo -u
	rosrun gazebo_path_plugin builder

6)Other Shell files were written for parking branch in Veronika's repository, you can clone the required repositeries and install this branch with ./init_sim_ws, don't forget to change 
	# Change to whereever you want
	ws_folder_path="/home/ostapvad/Documents/"
	folder_name="Phoenix_ws"
7)      ./run_parking_sim.sh - simulation in the parking mode
	./run_static_sim.sh  - simulation in the static mode

	Change in all shell files path to your workspace
