# Basic OpenGL with GLFW: Basic Particle System

This demo uses a modified spritebatcher from the previous example, adding the ability to rotate sprites.
Cpu based particle systems are relatively easy to create.
In this example, a class manages an array of particle structs, looping over and updating them.
Particles keep track of their age, and when they reach the maximum age, they are ignored, and later recycled.

# Setup

You will need to have CMake installed on your computer, and properly added to your path.
In order to setup, run the following in a shell, then open the project in your preferred editor.
Windows setup has been configured for use with Visual Studio.

Windows:
```
cd path/to/folder
setup.cmd
```
Linux:
```
cd path/to/folder
./setup
```
