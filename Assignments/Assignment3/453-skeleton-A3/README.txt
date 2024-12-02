CPSC 453: Assignment 3

Nathan Ante (30157706)
--------------------------------------------------------------

The program is entirely controlled by the GUI, except for the mouse which is used to control the camera position when viewing in 3D.

Mouse Controls:
- Scroll to zoom in/out
- Left click to select control points
- Left click drag to rotate camera

Universal GUI Inputs:
- Select background color
- Checkbox toggles for rendering control points and control point lines
- Reset camera

There is an ImGui Combo called program select to choose the mode of the program:
Curve Editor: Used to edit control points (no camera controls)
	- Point Mode Select: Select (drag), insert, delete point modes
		- Select: left click drag a point
		- Insert: left click anywhere in window (max 12 points)
		- Delete: left click point to delete
	- Curve Type Buttons: choose between Bezier or B-Spline curves
	- Reset points button to delete all points
Orbit Viewer: Used to view 2D curve in 3D camera
	- Use mouse controls to move camera position
	- Curve Type Buttons: choose between Bezier or B-Spline curves
Surface of Revolution: Creates a surface of revolution from generated curve in curve editor
	- Wireframe mode on/off
	- Surface parameters (slider bars): revolution slices, b-spline curve iterations
Tensor Product Surface: Used to render tensor product surfaces from predefined sets of control points
	- Wireframe mode on/off
	- Select Tensor Product Surface (2 buttons) to choose between default provided control points and my personal chosen sets of control points
	- B-spline curve iteration slider


--------------------------------------------------------------

Platform:
Windows 10/11 (works on both)

Compiler:
Microsoft (R) C/C++ Optimizing Compiler Version 19.41.34123 for x86

How to run:
Open folder in VS Code
Select 453-skeleton.exe as startup item