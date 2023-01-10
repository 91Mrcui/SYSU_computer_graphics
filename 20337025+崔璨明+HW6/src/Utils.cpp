#include "Utils.h"

#include <ctime>

Simulator::Simulator()
	: m_numParticles(0)
{
	//Position, velocity
	m_x.resize(m_maxNumParticles);
	m_v.resize(m_maxNumParticles);

	//Note: m_restLength[i][j]==0 means i and j are not connected
	m_restLength.resize(m_maxNumParticles, std::vector<float>(m_maxNumParticles, 0.0f));

	addNewParticle(0.3, 0.8);
	addNewParticle(0.4, 0.8);
	addNewParticle(0.5, 0.8);
	addNewParticle(0.6, 0.8);
	addNewParticle(0.7, 0.8);
	addNewParticle(0.75, 0.75);
	addNewParticle(0.75, 0.85);

}

void Simulator::drawRopeAndPoint(WindowsApp *winApp)
{
	//Visualization

	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		for (unsigned int j = i + 1; j < m_numParticles; ++j)
		{
			if (m_restLength[i][j] != 0)
			{
				int x0 = m_x[i].x * winApp->getWidth();
				int y0 = (1.0f - m_x[i].y) * winApp->getHeight();
				int x1 = m_x[j].x * winApp->getWidth();
				int y1 = (1.0f - m_x[j].y) * winApp->getHeight();
				winApp->drawLine(68, 85, 102, x0, y0, x1, y1);
			}
		}
	}

	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		int x = m_x[i].x * winApp->getWidth();
		int y = (1.0f - m_x[i].y) * winApp->getHeight();
		winApp->drawRectFill(255, 170, 119, x - 3, y - 3, 6, 6);
	}

}

void Simulator::addNewParticle(float x, float y)
{
	if (m_numParticles >= m_maxNumParticles)
		return;

	//Add a new particle to the system
	m_x[m_numParticles] = glm::vec2(x, y);
	m_v[m_numParticles] = glm::vec2(0, 0);
	
	//Connect with existing particles
	const auto &newParticle = m_x[m_numParticles];
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		float dist = glm::distance(m_x[i], newParticle);
		if (dist < m_connectRadius)
		{
			m_restLength[i][m_numParticles] = 0.1;
			m_restLength[m_numParticles][i] = 0.1;
		}
	}
	
	++m_numParticles;

}

void Simulator::simulate()
{
	static constexpr float dt = 0.0001f;
	static constexpr glm::vec2 gravity = glm::vec2(0.0f, -9.8f);
	//已经计算的速度
	std::vector<glm::vec2> m_v_prev(m_v);
	// TODO: for each particle i, calculate the force f, and then update the m_v[i]
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		// Gravity force
		glm::vec2 force = gravity * m_particleMass;
		// You should use m_restLength[i][j], m_stiffness, m_x, dt, and m_particleMass herein
		for (unsigned int j = 0; j < m_numParticles; ++j)
		{
			// 如果点不相连或是本身，跳过
			if (i == j || m_restLength[i][j] == 0) continue;
			// 弹力
			force += -m_stiffness *(glm::length(m_x[i] - m_x[j]) - m_restLength[i][j]) * glm::normalize(m_x[i] - m_x[j]);
		}
		//Update the m_v[i]
		m_v[i] += dt * force / m_particleMass;
	}

	// Collide with the ground
	// Note: no need to modify it
	for (unsigned int i = 0; i < m_numParticles; ++i)
	{
		if (m_x[i].y < 0.0f)
		{
			m_x[i].y = 0.0f;
			m_v[i].y = 0.0f;
		}
	}

	// Todo: update the position m_x[i] using m_v[i]
	// Note: you should use the time step dt
	for (unsigned int i = 1; i < m_numParticles; ++i)
	{
		m_v[i] *= glm::exp(-dt * m_damping);
		m_x[i] += dt * m_v[i];
	}
}