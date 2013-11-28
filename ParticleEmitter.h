#pragma once

#include <Windows.h>
#include <vector>

#include "library.h"
#include "Particle.h"

namespace particle_system
{

	// PARAMETERS
	const int NB_PARTICLES = 50;
	const float PARTICLE_SIZE = 0.005f;
	const float INIT_LIFESPAN = 3.0f; // 3 seconds
	const float VELOCITY = 0.5f;

	GLuint m_shader;
	float m_lastUpdate;
	std::vector<Particle*> m_particles;


	void construct()
	{
		m_lastUpdate = 0;
		m_shader = CreateShaderProgram("bpl_source/color_part.vert", "bpl_source/color_part.frag");
	}

	void update()
	{
		float tmp = (float)GetTickCount() / 1000.0f;
		float deltaTime;
		if (m_lastUpdate == 0){
			deltaTime = 0;
		}
		else{
			deltaTime = tmp - m_lastUpdate;
		}
		m_lastUpdate = tmp;

		int size = m_particles.size();
		int i(0);
		while (i < size)
		{
			Particle* p = m_particles[i];
			if (p->getLifespan() <= 0){
				m_particles.erase(m_particles.begin() + i);
				size--;
			}
			else{
				p->update(deltaTime);
				//      std::cout << "[i = " << i << "] x: " << p->pos.x << ", y: " << p->pos.y << std::endl;
			}
			i++;
		}
	}

	glm::vec2 randomVelocity()
	{
		// angle in radians
		float angle = (rand() / (float)RAND_MAX) * 2 * 3.1415926535f;
		float xCoef = (rand() / (float)RAND_MAX);
		float yCoef = (rand() / (float)RAND_MAX);
		return glm::vec2(cos(angle)*xCoef * VELOCITY, sin(angle)*yCoef * VELOCITY);
	}

	glm::vec3 randomRotation()
	{
		float x = (float)rand() / RAND_MAX;
		float y = (float)rand() / RAND_MAX;
		float z = (float)rand() / RAND_MAX;
		return glm::vec3(x, y, z);
	}

	float randomAngle()
	{
		return ((float)rand() / RAND_MAX) * 40.0 - 20.0;
	}

	void addParticle(glm::vec2 pos)
	{
		Particle* p;
		if (rand() % 2){
			p = new ParticleOne(pos, randomVelocity(), INIT_LIFESPAN,
								rand() % 2, randomRotation(), randomAngle(),
								PARTICLE_SIZE);
		}
		else{
			p = new ParticleZero(pos, randomVelocity(), INIT_LIFESPAN,
								rand() % 2, randomRotation(), randomAngle(),
								PARTICLE_SIZE);
		}
		m_particles.push_back(p);
	}


	void addSource(int mx, int my)
	{
		float s = (float)mx / (float)window_width*2.0f - 1.0f;
		float t = (float)my / (float)window_height*2.0f - 1.0f;

		//float screenX = 2 * (float)x / window_width - 1;
		//float screenY = 1 - 2 * (float)y / window_height;

		for (int i = 0; i < NB_PARTICLES; i++) {
			addParticle(glm::vec2(s, t));
		}
	}

	void draw()
	{
		glm::mat4 projection;
		//projection = glm::perspective(45.0, (double)window_width / window_height, 1.0, 100.0);
		projection = glm::ortho(-1.0, 1.0, +1.0, -1.0, 0.1, 100.0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (int i(0); i < m_particles.size(); i++)
		{
			m_particles[i]->draw(projection, m_shader);
		}

		glDisable(GL_BLEND);
	}



};

