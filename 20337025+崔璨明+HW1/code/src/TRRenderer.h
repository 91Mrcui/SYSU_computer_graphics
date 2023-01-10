#ifndef TRRENDERER_H
#define TRRENDERER_H

#include "glm/glm.hpp"
#include "SDL2/SDL.h"

#include "TRFrameBuffer.h"
#include "TRDrawableMesh.h"
#include "TRShaderPipeline.h"

#include <mutex>

namespace TinyRenderer
{
	class TRRenderer final
	{
	public:
		typedef std::shared_ptr<TRRenderer> ptr;

		TRRenderer(int width, int height);
		~TRRenderer() = default;

		void loadDrawableMesh(const std::string &filename);
		void unloadDrawableMesh();

		void clearColor(glm::vec4 color);

		void setViewMatrix(const glm::mat4 &view);
		void setModelMatrix(const glm::mat4 &model);
		void setProjectMatrix(const glm::mat4 &project);
		void setShaderPipeline(TRShaderPipeline::ptr shader);

		glm::mat4 getMVPMatrix();

		void renderAllDrawableMeshes();

		unsigned char* commitRenderedColorBuffer();

		//Auxiliary function
		static glm::mat4 calcViewPortMatrix(int width, int height);
		static glm::mat4 calcViewMatrix(glm::vec3 camera, glm::vec3 target, glm::vec3 worldUp);
		static glm::mat4 calcPerspProjectMatrix(float fovy, float aspect, float near, float far);
		static glm::mat4 calcOrthoProjectMatrix(float left, float right, float bottom, float top, float near, float far);

	private:
		bool isTowardBackFace(const glm::vec4 &v0, const glm::vec4 &v1, const glm::vec4 &v2) const;

	private:

		//Drawable mesh array
		std::vector<TRDrawableMesh> m_drawableMeshes;

		//MVP transformation matrices
		glm::mat4 m_viewMatrix = glm::mat4(1.0f);
		glm::mat4 m_modelMatrix = glm::mat4(1.0f);
		glm::mat4 m_projectMatrix = glm::mat4(1.0f);
		glm::mat4 m_mvp_matrix = glm::mat4(1.0f);
		bool m_mvp_dirty = false;

		//Viewport transformation (ndc space -> screen space)
		glm::mat4 m_viewportMatrix = glm::mat4(1.0f);

		//Shader pipeline handler
		TRShaderPipeline::ptr m_shader_handler = nullptr;

		//Double buffers
		TRFrameBuffer::ptr m_backBuffer;                      // The frame buffer that's goint to be written.
		TRFrameBuffer::ptr m_frontBuffer;                     // The frame buffer that's goint to be display.
	};
}

#endif