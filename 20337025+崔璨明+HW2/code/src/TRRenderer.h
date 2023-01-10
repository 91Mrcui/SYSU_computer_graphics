#ifndef TRRENDERER_H
#define TRRENDERER_H

#include "glm/glm.hpp"
#include "SDL2/SDL.h"

#include "TRFrameBuffer.h"
#include "TRDrawableMesh.h"
#include "TRShaderPipeline.h"
#include<vector>

#include <mutex>

struct clip_plane {
	glm::vec4 P;//平面
	glm::vec4 n;//法线
};

namespace TinyRenderer
{
	class TRRenderer final
	{
	public:
		typedef std::shared_ptr<TRRenderer> ptr;

		TRRenderer(int width, int height);
		~TRRenderer() = default;

		//Drawable objects load/unload
		void loadDrawableMesh(const std::string &filename);
		void unloadDrawableMesh();

		void clearColor(glm::vec4 color);

		//Setting
		void setViewMatrix(const glm::mat4 &view);
		void setModelMatrix(const glm::mat4 &model);
		void setProjectMatrix(const glm::mat4 &project, float near, float far);
		void setShaderPipeline(TRShaderPipeline::ptr shader);

		glm::mat4 getMVPMatrix();

		//Draw call
		void renderAllDrawableMeshes();

		//Commit rendered result
		unsigned char* commitRenderedColorBuffer();
		unsigned int getNumberOfClipFaces() const;
		unsigned int getNumberOfCullFaces() const;

		//Auxiliary function
		static glm::mat4 calcViewPortMatrix(int width, int height);
		static glm::mat4 calcViewMatrix(glm::vec3 camera, glm::vec3 target, glm::vec3 worldUp);
		static glm::mat4 calcPerspProjectMatrix(float fovy, float aspect, float near, float far);
		static glm::mat4 calcOrthoProjectMatrix(float left, float right, float bottom, float top, float near, float far);

		
		
	private:

		//Homogeneous space clipping - Sutherland Hodgeman algorithm
		std::vector<TRShaderPipeline::VertexData> cliping(
			const TRShaderPipeline::VertexData &v0,
			const TRShaderPipeline::VertexData &v1,
			const TRShaderPipeline::VertexData &v2) const;

		std::vector<TRShaderPipeline::VertexData> clip_with_plane(
			clip_plane& c_plane, std::vector<TRShaderPipeline::VertexData>& vert_list) const;


		//Back face culling
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

		//Near plane & far plane
		glm::vec2 m_frustum_near_far;

		//Viewport transformation (ndc space -> screen space)
		glm::mat4 m_viewportMatrix = glm::mat4(1.0f);

		//Shader pipeline handler
		TRShaderPipeline::ptr m_shader_handler = nullptr;

		//Double buffers
		TRFrameBuffer::ptr m_backBuffer;                      // The frame buffer that's goint to be written.
		TRFrameBuffer::ptr m_frontBuffer;                     // The frame buffer that's goint to be display.

		struct Profile
		{
			unsigned int m_num_cliped_triangles = 0;
			unsigned int m_num_culled_triangles = 0;
		};
		Profile m_clip_cull_profile;
	};
}

#endif