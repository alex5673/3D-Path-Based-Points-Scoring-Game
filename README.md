# 3D-Path-Based-Points-Scoring-Game

Third year Computer Graphics project.

This game demo is a basic point scoring game in which the player is continously moving a path, collecting wooden boxes and golden rings to 
score points and avoid jeeps that make the player lose a life when hit. The player has 3 lives and when the player loses all of their lives 
it is game over. The player can switch between 3rd-person and top down views.

The path is generated using a Catmull-Rom spline and the player is moved along the points in the splines. A TNB frame is then specified to 
work out the orientation of the player and define the vectors that allow the player to move left and right on the path to collect pickups 
and avoid obstacles.

The graphics are implemented using OpenGL with basic Vertex and Fragment shaders. All pickups are made from basic primitives and texture 
mapped. Meshes (rhino representing the player and the jeep representing the obstacles) are imported using OpenAssetImporter.
