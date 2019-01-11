cmake_minimum_required( VERSION 3.5 )

project( console_app )

# include files
include_directories( ./include ./src)

# target
add_executable( binary src/main.cpp src/Agents.cpp src/Environment.cpp src/GameLauncher.cpp )
