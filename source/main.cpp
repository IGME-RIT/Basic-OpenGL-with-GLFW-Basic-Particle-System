/*
Title: Sprite Batching
File Name: main.cpp
Copyright � 2016
Author: David Erbelding
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "FreeImage.h"
#include <vector>
#include "../header/shader.h"
#include "../header/spriteBatcher.h"
#include "../header/particleSystem.h"
#include <iostream>

glm::vec2 viewportDimensions = glm::vec2(800, 600);
SpriteBatcher* spriteBatcher;
glm::vec2 mousePosition;
ParticleSystem* particleSystem;

// Window resize callback
void resizeCallback(GLFWwindow* window, int width, int height)
{
    viewportDimensions = glm::vec2(width, height);
    glViewport(0, 0, width, height);
    spriteBatcher->SetScreenSize(viewportDimensions);
}

// This will get called when the mouse moves.
void mouseMoveCallback(GLFWwindow *window, GLdouble mouseX, GLdouble mouseY)
{
    mousePosition = glm::vec2(mouseX, mouseY);
}

// Change things when keys get pressed
void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        particleSystem->m_useLocalCoordinates = !particleSystem->m_useLocalCoordinates;
    }
    if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    {
        particleSystem->m_startVelocityRange -= 50;
    }
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        particleSystem->m_startVelocityRange += 50;
    }
    if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    {
        if (particleSystem->m_lifeTime > .25f) particleSystem->m_lifeTime -= .25f;
    }
    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
    {
        particleSystem->m_lifeTime += .25f;
    }
    if (key == GLFW_KEY_G && action == GLFW_RELEASE)
    {
        particleSystem->m_acceleration.y -= 100;
    }
    if (key == GLFW_KEY_H && action == GLFW_RELEASE)
    {
        particleSystem->m_acceleration.y += 100;
    }
}

int main(int argc, char **argv)
{
	// Initializes the GLFW library
	glfwInit();

	// Initialize window
	GLFWwindow* window = glfwCreateWindow(viewportDimensions.x, viewportDimensions.y, "Particles!", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	//set resize callback and mouse movement callback
	glfwSetFramebufferSizeCallback(window, resizeCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetKeyCallback(window, keyPressCallback);
	// Initializes the glew library
	glewInit();


    Texture* texture = new Texture("../assets/particle.png");
    texture->IncRefCount();

    // Create a spriteBatcher (This one will need the size of the screen)
    spriteBatcher = new SpriteBatcher(glm::vec2(800, 600));
    
    // Initialize the particle system class with a bunch of parameters:
    particleSystem = new ParticleSystem(texture);
    particleSystem->m_position = glm::vec2(100, 0);
    particleSystem->m_useLocalCoordinates = false;
    particleSystem->m_spawnFrequency = .01;
    particleSystem->m_lifeTime = 1;
    particleSystem->m_startVelocityMin = 0;
    particleSystem->m_startVelocityRange = 200;
    particleSystem->m_rotationRange = glm::vec2(0, sin(1) * 2);
    particleSystem->m_angularVelocityRange = glm::vec2(-5, 10);
    particleSystem->m_acceleration = glm::vec2(0, 0);



    std::cout << "Controls:" << std::endl;
    std::cout << "Press space to toggle particle local space." << std::endl;
    std::cout << "Press A and S to decrease and increase initial particle velocity." << std::endl;
    std::cout << "Press D and F to decrease and increase particle duration." << std::endl;
    std::cout << "Press G and H to decrease and increase gravity." << std::endl;


    float frames = 0;
    float secCounter = 0;

	// Main Loop
	while (!glfwWindowShouldClose(window))
	{
        // Calculate delta time and frame rate
        float dt = glfwGetTime();
        frames++;
        secCounter += dt;
        if (secCounter > 1.f)
        {
            std::string title = "FPS: " + std::to_string(frames);
            glfwSetWindowTitle(window, title.c_str());
            secCounter = 0;
            frames = 0;
        }
        glfwSetTime(0);



        // Convert the mouse position on screen to world coordinates
        glm::vec2 direction = mousePosition;
        direction.y = viewportDimensions.y - direction.y;

        particleSystem->m_position = direction;
        particleSystem->Update(dt);

		// Clear the screen.
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 0.0);

        particleSystem->Draw(spriteBatcher);
                    
        // Now that we have a collection of all the draws we want to make, send it all to the gpu to be drawn!
        spriteBatcher->Flush();

		// Swap the backbuffer to the front.
		glfwSwapBuffers(window);

		// Poll input and window events.
		glfwPollEvents();

	}

    delete spriteBatcher;
    delete particleSystem;

    texture->DecRefCount();

	// Free GLFW memory.
	glfwTerminate();


	// End of Program.
	return 0;
}
