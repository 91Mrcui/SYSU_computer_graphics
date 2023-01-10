#ifndef UTILS_H
#define UTILS_H

#include <vector>

#include "WindowsApp.h"
#include "glm/glm.hpp"

class Simulator
{
public:

	Simulator();
	~Simulator() = default;

	void addNewParticle(float x, float y);
	void simulate();

	void drawRopeAndPoint(WindowsApp *winApp);

private:	
	unsigned int m_numParticles;
	const unsigned int m_maxNumParticles = 256;

	std::vector<glm::vec2> m_x;
	std::vector<glm::vec2> m_v;
	std::vector<std::vector<float>> m_restLength;

	float m_damping = 5.0f;
	float m_stiffness = 8000;
	const float m_particleMass = 1.0f;
	const float m_connectRadius = 0.15f;

};

#endif