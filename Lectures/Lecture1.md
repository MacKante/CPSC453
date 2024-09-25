# Lecture 1

#### September 5, 2024

---

## Introduction

### Grading
- **Weekly Quiz:** 20%
    - 10 weekly quizzes every Thursday (last 30 minutes of class)
    - Best eight will be counted
    - Bring calculator
- **Assignments:** 40%
    - 5 programming assignments
    - Optional 5th assignment with a bonus mark
    - 6 Days (no questions asked) extension bank, max 3 days per assignment
    - Roughly due every 2-3 weeks
    - Boiler plate code given
    - Attend tutorials and ask help from TAs 
- **Final Exam:** 40%
    - In-person


### Teaching style (lectures)
- Slides are useful but are **NOT** notes
- Important material written on course notes/board

### Textbook
**Recommended (not required)**:
- E. Angels **"Interactive Computer Graphics, A Top-Down Approach Using OpenGL"** 7th Ed. 2015
- P. Shirley **"Fundamentals of Computer Graphics"** 2015

### What is Computer Graphics?
- Science/tools/techniques/algorithms to generate pictures on a computer (basic definition)
- Interactive visual
- Augmenting visual interactions, *R. Cook*
- Augmenting our imagination, *Jim Kajaya*

### How to create images?
- **Modelling (form)**
    - How to represent objects
    - main focus (basic coverage)
    - CPSC 589
- **Render (appearance)**
    - how to add shading and lighting
    - main focus (basic coverage)
    - CPSC 591
- **Animation (behaviour)**
    - How to move objects
    - Will touch lightly on
    - CPSC 587

### What is this course about?
- **It is not about...**
    - paint and imaging packages (photoshop)
    - CAD packages (AutoCAD)
    - graphics packages (Blender, 3D Studio Max, MAYA) in graphics hardware design
    - API (DirectX, OpenGL, Vulkan...)
        - OpenGL will be covered in labs
    - GUI
        - basics covered in labs
- **It is about...**
    - how to create and develop graphics software
        - algorithms, programming, data structures
    - learn computer graphics fundamentals

### Fun course, but...
- **Prerequisites are essential**
    - Programming
    - Data structure
    - Linear algebra
    - Introductory calculus and geometry

### Why Computer Graphics?!
- Wide range applications
    - GIS
    - Game
    - CAD/CAM
    - Film
    - Virtual and augmented reality
    - Fabrication
    - Information and Scientific Visualization
- Cool! (aesthetic factor)
- Clout

## Part 2: Graphics Systems

### Common Graphic System
![A typical graphics system](../z_ImageDump/A%20typical%20graphics%20system.jpg)

### Raster Displays
- CRT, LCD, LED, OLED displays
- Show raster images (?) line by line
- Needs refreshing (60-120 times per second)
- Raster Images: rectangular grid of ***"pixels"***
    - **Pixels:** an area of the image with a constant color (discrete samples)

### Pixel Values
- Representing colors
- RGB (a, b, c)
    - [0 , 1] -> intensity of color (0 for none, 1 for max intensity)
    - Examples:
        - (1, 0, 0) -> Red
        - ( 0, 1, 0) -> Green
        - (1, 1, 1) -> White
        - (0, 0, 0) -> Black
        - (0.9, 0, 0) -> Darker Red
    - Closer to zero is less of that color
    - Closer to one is more of that color
- Storing color
    - 8 bits instead of floating points [0, 255]
    - 8 bits per color component

### Common Graphic Pipeline
![A typical graphics pipeline using OpenGL API](../z_ImageDump/A%20typical%20graphics%20pipeline%20using%20OpenGL%20API.jpg)

- Application Program Interface (API):
    - Process the scene data
    - Vertex processing to form geometric primitives
    - (Example) OpenGL

- Graphics Hardware:
    - Process the data and transmit the visual output to the graphics hardware, usually raster displays
    - (Example) Graphics Processing Unit (GPU)

### GPU (Graphics Processing Unit): TBC


