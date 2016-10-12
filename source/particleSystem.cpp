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

#include "../header/particleSystem.h"

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
    m_internalTimer += dt;

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        // Get a reference to that particle, not a copy.
        Particle& p = m_particles[i];

        // Particle is alive, update it
        if (p.m_age > 0)
        {
            p.m_age -= dt;
            p.m_position += p.m_velocity * dt;
            p.m_velocity += m_acceleration * dt;
            p.m_rotation += p.m_angularVelocity * dt;
        }
        // Particle is dead, and we can spawn
        else if (m_internalTimer > m_spawnFrequency)
        {
            m_internalTimer -= m_spawnFrequency;
            glm::vec2 initialVelocity = glm::vec2(
                rand() / (float)RAND_MAX - .5f,
                rand() / (float)RAND_MAX - .5f
            );
            float speed = rand() / (float)RAND_MAX * m_startVelocityRange + m_startVelocityMin;
            initialVelocity = glm::normalize(initialVelocity) * speed;
            float initialRotation = (rand() / (float)RAND_MAX) * m_rotationRange.y + m_rotationRange.x;
            float initialAngular = (rand() / (float)RAND_MAX) * m_angularVelocityRange.y + m_angularVelocityRange.x;

            if(m_useLocalCoordinates) p.m_position = glm::vec2(0);
            else p.m_position = m_position;

            p.m_velocity = initialVelocity;
            p.m_rotation = initialRotation;
            p.m_angularVelocity = initialAngular;
            p.m_age = m_lifeTime;
            p.m_color = glm::vec4(rand() % 2, rand() % 2, rand() % 2, 1);
        }
    }
}

void ParticleSystem::Draw(SpriteBatcher* spriteBatcher)
{

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);


    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Particle p = m_particles[i];
        // Particle is alive, draw it
        if (p.m_age > 0)
        {
            glm::vec2 v = p.m_position;
            if (m_useLocalCoordinates) v += m_position;

            glm::vec4 color = p.m_color;
            color.a = p.m_age / m_lifeTime;

            spriteBatcher->DrawRotated(
                glm::vec4(v, 64, 64), // sprite destination and size
                glm::vec4(0, 0, 128, 128), // sprite source
                color, // color
                m_texture,
                glm::vec2(32, 32), // rotation axis and angle
                p.m_rotation);
        }
    }
    spriteBatcher->Flush();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
