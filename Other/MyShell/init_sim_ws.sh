#!/bin/bash

# A simple script for setting your catkin workspace and all required files

# Change to whereever you want
ws_folder_path="/home/ostapvad/Documents/"
folder_name="Phoenix_ws"

init_ws(){
    # initiate catkin workspace
    cd $ws_folder_path
    mkdir $folder_name
    cd $folder_name
    mkdir src
    catkin_make
}

clone_reps(){
    # clones all required repositories
    gnome-terminal
    cd $ws_folder_path/$folder_name/src
    git init
    git clone -b parking git@github.com:peconver/skoda_simulation.git
    git clone -b simulation git@github.com:gestom/phoenix.git
    cd ..
    catkin_make # one catkin_make throws error
}

#init_ws
clone_reps

exec bash