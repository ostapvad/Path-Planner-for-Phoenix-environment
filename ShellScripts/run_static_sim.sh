#!/bin/bash
# A simple shell script for running the simulation in static mode
set_ws(){
    # Move to your ROS workspace 
    ws_path="/home/ostapvad/Documents/Phoenix_ws" # path to ROS workpace
    current_path="$(pwd)"
    if [ "$ws_path" = "$current_path" ];then
        echo "You are in your ROS workspace!"
    else
        cd $ws_path
        echo "You were moved to your ROS workspace!"
    fi
}
set_tmux(){
    # Set tmux for further using
    tmux kill-server
    session_name="Static"
    windows_names=("Main node" "Gazebo" "Model launcher")
    commands=("roscore" 
              "rosrun gazebo_ros gazebo src/skoda_simulation/src/ackermannsteer/steerbot/world/world.world"
              "roslaunch steerbot world.launch")

    # Set the windows
    tmux new-session -d -s $session_name -n "${windows_names[0]}"
    
    # in case of errors
    #tmux send-keys -t $session_name:"${windows_names[0]}" "pkill gzserver && pkill gzclient" C-m 
    tmux new-window -n "${windows_names[1]}" -t $session_name
    tmux split-window -v
    tmux new-window -n "${windows_names[2]}" -t $session_name
    
    # Run the commands
    for i in {0..2}
    do  
        tmux send-keys -t $session_name:"${windows_names[i]}" "${commands[i]}" C-m
        sleep 0.5s
    done
    # Go to Gazebo window
    tmux ls
    tmux select-window -t 1
    tmux select-pane -t 0
    tmux a -t $session_name


}

set_ws
set_tmux


exec bash

