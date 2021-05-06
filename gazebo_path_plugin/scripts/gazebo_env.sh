#!/usr/bin/env sh
export GAZEBO_PLUGIN_PATH="${CATKIN_ENV_HOOK_WORKSPACE}/lib"
export GAZEBO_MODEL_PATH="$(rospack find gazebo_path_plugin)/models"
