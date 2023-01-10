#ifndef TRSHADING_STATE_H
#define TRSHADING_STATE_H

#include "glm/glm.hpp"

namespace TinyRenderer
{
	//Texture warping mode
	enum TRTextureWarpMode
	{
		TR_REPEAT,
		TR_CLAMP_TO_EDGE	
	};

	//Texture filtering mode
	enum TRTextureFilterMode
	{
		TR_NEAREST,
		TR_LINEAR
	};

	//Polygon mode
	enum TRPolygonMode
	{
		TR_TRIANGLE_WIRE,
		TR_TRIANGLE_FILL
	};

	//Cull back face mode
	enum TRCullFaceMode
	{
		TR_CULL_DISABLE,
		TR_CULL_FRONT,
		TR_CULL_BACK
	};

	enum TRDepthTestMode
	{
		TR_DEPTH_TEST_DISABLE,
		TR_DEPTH_TEST_ENABLE
	};

	enum TRDepthWriteMode
	{
		TR_DEPTH_WRITE_DISABLE,
		TR_DEPTH_WRITE_ENABLE
	};

	enum TRLightingMode
	{
		TR_LIGHTING_DISABLE,
		TR_LIGHTING_ENABLE
	};

	//Point lights
	class TRPointLight
	{
	public:
		glm::vec3 lightPos;//Note: world space position of light source
		glm::vec3 attenuation;
		glm::vec3 lightColor;

		TRPointLight(glm::vec3 pos, glm::vec3 atten, glm::vec3 color)
			: lightPos(pos), attenuation(atten), lightColor(color) {}
	};

	//Task5
	class TRSpotLight
	{
	public:
		glm::vec3 lightPos;//world space
		glm::vec3 direction;//direction
		float cutoff;//���н�
		float outcutoff;//���н�

		TRSpotLight(glm::vec3 pos, glm::vec3 dir, float cutoff, float outcutoff)
			: lightPos(pos), direction(dir), 
			cutoff(cutoff), outcutoff(outcutoff) {}
	};
}

#endif