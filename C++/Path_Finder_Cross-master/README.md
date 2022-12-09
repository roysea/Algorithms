# Path_Finder_Cross
This is a path_finding algorithm designed to find the best path using the A* algorithm in a 2d grid  

SFML is used to draw the grid and the files for windows or mac are located inside the ext_libs folder.  

CS-265 Data Structures and Algorithms in C++  

To use:  
1) Link SFML to your project using SFML's instructions on sfml-dev.org
2) Set your workspace directory to your projects folder
3) Link /src/ and /res/ to the project
4) Run and compile as x64 as x86 is not compatible with the version of SFML linked

Grid Controls:  
R: Creates a random grid layout. Each Node on the grid has a 33% chance of becoming a wall  
I: Reinitializes the grid. (Makes the grid completely blank)  
C: Clears everything on the grid besides the walls  
T: Runs the test function which will loop over 1000 iterations of random grid layouts and solves each random grid layout  
Enter: Runs the A* Pathfinder  
  
Window Controls: (All of the following use are on the arrow keys)  
Right: Increases the grid width by 1  
Left: Decreases the grid width by 1  
Up: Increases the grid height by 1  
Down: Decreases the grid height by 1  
  
Hotkeys for new brush:  
W: Wall  
S: Start  
D: Destination  
Left Click: Draws on the grid  
