/*
Title: Sprite Batching
File Name: particleSystem.cpp
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

#include "particleSystem.h"

ParticleSystem::ParticleSystem(Texture* texture)
{
    m_texture = texture;
    m_texture->IncRefCount();
}

ParticleSystem::~ParticleSystem()
{
    m_texture->DecRefCount();
}

void ParticleSystem::Update(float dt)
{
    // This timer is used to keep track of how often to spawn new particles.
    m_internalTimer += dt;

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = m_particles[i];

        // Particle is alive, update its position and rotation etc.
        if (p.m_age > 0)
        {
            p.m_age -= dt;
            p.m_position += p.m_velocity * dt;
            p.m_velocity += m_acceleration * dt;
            p.m_rotation += p.m_angularVelocity * dt;
        }
        // Particle is dead, and we can spawn a new one if one hasn't already been spawned recently
        else if (m_internalTimer > m_spawnFrequency)
        {
            // Decrement this timer by the frequency, so that this test will eventually pass if the spawner has caught up with delta time.
            m_internalTimer -= m_spawnFrequency;

            // Calculate a random direction and speed for an initial particel velocity
            float direction = (float)rand();
            glm::vec2 initialVelocity = glm::vec2(cos(direction), sin(direction));
            float speed = rand() / (float)RAND_MAX * m_startVelocityRange + m_startVelocityMin;
            initialVelocity = initialVelocity * speed;

            // Random rotation and angular velovity
            float initialRotation = (rand() / (float)RAND_MAX) * m_rotationRange.y + m_rotationRange.x;
            float initialAngular = (rand() / (float)RAND_MAX) * m_angularVelocityRange.y + m_angularVelocityRange.x;

            // In local coordinates, we want to start the particle at 0.
            // In world coordinates, we spawn it at the particle system location, and let it do its own thing.
            if(m_useLocalCoordinates) p.m_position = glm::vec2(0);
            else p.m_position = m_position;

            // Set the particle up
            p.m_velocity = initialVelocity;
            p.m_rotation = initialRotation;
            p.m_angularVelocity = initialAngular;
            p.m_color = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);
            // Set the particle timer back up so that it will update.
            p.m_age = m_lifeTime;
        }
    }
}

void ParticleSystem::Draw(SpriteBatcher* spriteBatcher)
{
    // Enable blending when rendering particles
    glEnable(GL_BLEND);
    // SRC_ALPHA, and ONE give additive blending
    // Switch to SRC_ALPHA, ONE_MINUS_SRC_ALPHA to make particles stack on top of eachother.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Particle p = m_particles[i];
        // Particle is alive, draw it
        if (p.m_age > 0)
        {
            // Add the system position to the particle position if using local coordinates
            glm::vec2 v = p.m_position;
            if (m_useLocalCoordinates) v += m_position;

            // Make the particle fade in alpha until it disappears
            glm::vec4 color = p.m_color;
            color.a = p.m_age / m_lifeTime;

            // Queue it with the spritebatcher
            spriteBatcher->DrawRotated(
                glm::vec4(v, 64, 64), // sprite destination and size
                glm::vec4(0, 0, 128, 128), // sprite source
                color, // color
                m_texture,
                glm::vec2(32, 32), // rotation axis and angle
                p.m_rotation);
        }
    }
    // Draw all the particles at once.
    spriteBatcher->Flush();

    glDisable(GL_BLEND);
}
