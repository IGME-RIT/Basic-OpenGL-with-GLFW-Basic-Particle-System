/*
Title: Sprite Batching
File Name: particleSystem.h
Copyright ? 2016
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

#pragma once
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "spriteBatcher.h"
struct Particle
{
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    glm::vec4 m_color;
    float m_rotation;
    float m_angularVelocity;
    float m_age;
};


class ParticleSystem
{
public:
    ParticleSystem(Texture* texture);
    ~ParticleSystem();

    void Update(float dt);
    void Draw(SpriteBatcher* spriteBatcher);

    // Particles move relative to the system or relative to the world.
    bool m_useLocalCoordinates = true;
    // Position of the system.
    glm::vec2 m_position;

    // Lower number means higher spawn rate
    float m_spawnFrequency = .05f;
    // Time in seconds until particles are recycled.
    float m_lifeTime = 1.f;

    // lowest possible starting particle speed
    float m_startVelocityMin = 0;
    // highest speed relative to lowest
    float m_startVelocityRange = 100;

    // possible starting rotations for particles. x is lowest. y is range.
    glm::vec2 m_rotationRange = glm::vec2(0, 0);
    // possible angular velocity same system as rotation range
    glm::vec2 m_angularVelocityRange = glm::vec2(0, 0);

    // global acceleration applied to all particles, defaults to 0
    glm::vec2 m_acceleration = glm::vec2(0, 0);

private:
    // The particle system will work with a predefined pool of 512 particles, this makes things way faster than having a dynamic list.
    static const int MAX_PARTICLES = 512;
    Particle m_particles[MAX_PARTICLES];

    Texture* m_texture;
    float m_internalTimer = 0;

};