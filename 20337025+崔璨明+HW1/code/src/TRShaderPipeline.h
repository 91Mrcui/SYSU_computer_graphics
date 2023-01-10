#ifndef TRSHADERPIPELINE_H
#define TRSHADERPIPELINE_H

#include <vector>
#include <memory>

#include "glm/glm.hpp"

namespace TinyRenderer
{
	class TRShaderPipeline
	{
	public:
		typedef std::shared_ptr<TRShaderPipeline> ptr;
		
		class VertexData
		{
		public:
			glm::vec4 pos;
			glm::vec3 col;
			glm::vec3 nor;
			glm::vec2 tex;
			glm::vec4 cpos; //Clip space position
			glm::ivec2 spos; //Screen space position

			//Linear interpolation
			static VertexData lerp(const VertexData &v0, const VertexData &v1, float frac);

			//Perspective correction for interpolation
			static void prePerspCorrection(VertexData &v);
			static void aftPrespCorrection(VertexData &v);
		};

		virtual ~TRShaderPipeline() = default;

		void setModelMatrix(const glm::mat4 &model) { m_model_matrix = model; }
		void setViewProjectMatrix(const glm::mat4 &vp) { m_view_project_matrix = vp; }

		//Shaders
		virtual void vertexShader(VertexData &vertex) = 0;
		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) = 0;

		//Rasterization
		static std::vector<VertexData> rasterize_wire(
			const VertexData &v0,
			const VertexData &v1,
			const VertexData &v2,
			const unsigned int &screen_width,
			const unsigned int &screen_height);
		static std::vector<VertexData> rasterize_fill(
			const VertexData &v0,
			const VertexData &v1,
			const VertexData &v2,
			const unsigned int &screen_width,
			const unsigned int &screen_height);

	protected:

		//Auxiliary function
		static std::vector<VertexData> rasterize_wire_aux(
			const VertexData &begin,
			const VertexData &end,
			const unsigned int &screen_width,
			const unsigned int &screen_height);

		glm::mat4 m_model_matrix = glm::mat4(1.0f);
		glm::mat4 m_view_project_matrix = glm::mat4(1.0f);

	};

	class TRDefaultShaderPipeline final : public TRShaderPipeline
	{
	public:

		typedef std::shared_ptr<TRDefaultShaderPipeline> ptr;

		virtual ~TRDefaultShaderPipeline() = default;

		virtual void vertexShader(VertexData &vertex) override;

		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) override;

	};
}

#endif