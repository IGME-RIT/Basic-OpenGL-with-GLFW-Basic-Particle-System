Documentation Author: Niko Procopi 2019

This tutorial was designed for Visual Studio 2017 / 2019
If the solution does not compile, retarget the solution
to a different version of the Windows SDK. If you do not
have any version of the Windows SDK, it can be installed
from the Visual Studio Installer Tool

Welcome to the 2D Particles Tutorial!
Prerequesites: Intro Graphics, SpriteBatch

In this tutorial, we process all particles on the CPU,
and then we make one draw call of each particle. Each
particle (quad) is sent to the pipeline, one at a time,
going through vertex shader, then fragment shader, then screen.

This tutorial draws particles with the SpriteBatch 
system that was made in the 2D tutorials.

The two most important functions here are in particleSystem.cpp

Update() function loops through every particle individually,
and chooses whether to update an existing particle, or
recycle an old particle into a new one (by respawning it).
While updating, each existing particle has properties that 
change each frame: age, position, velocity, and rotation.

When spawning a new particle, we calculate new initial
values for velocity, position, and angle, based on rand(),
for random number generation.

When it is time to draw each particle, we loop through
the list of particles in the Draw() functin of 
particleSystem.cpp, and we call spriteBatcher->DrawRotated()
for every particle, one at a time, which is slow.

How to improve:
Draw all particles at once, with Instanced Rendering