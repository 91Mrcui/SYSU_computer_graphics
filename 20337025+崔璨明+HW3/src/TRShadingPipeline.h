#ifndef TRSHADERPIPELINE_H
#define TRSHADERPIPELINE_H

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "TRTexture2D.h"

namespace TinyRenderer
{
	class TRShadingPipeline
	{
	public:
		typedef std::shared_ptr<TRShadingPipeline> ptr;
		
		class VertexData
		{
		public:
			glm::vec4 pos;  //World space position
			glm::vec3 col;  //World space color
			glm::vec3 nor;  //World space normal
			glm::vec2 tex;	//World space texture coordinate
			glm::vec4 cpos; //Clip space position
			glm::ivec2 spos;//Screen space position
			glm::mat3 TBN;  //Tangent, bitangent, normal matrix

			//Linear interpolation
			static VertexData lerp(const VertexData &v0, const VertexData &v1, float frac);
			static VertexData barycentricLerp(const VertexData &v0, const VertexData &v1, const VertexData &v2, const glm::vec3 &w);

			//Perspective correction for interpolation
			static void prePerspCorrection(VertexData &v);
			static void aftPrespCorrection(VertexData &v);
		};

		virtual ~TRShadingPipeline() = default;

		//Vertex shader settting
		void setModelMatrix(const glm::mat4 &model) 
		{ 
			m_model_matrix = model;
			//Refs: https://learnopengl-cn.github.io/02%20Lighting/02%20Basic%20Lighting/
			m_inv_trans_model_matrix = glm::mat3(glm::transpose(glm::inverse(m_model_matrix)));
		}
		void setViewProjectMatrix(const glm::mat4 &vp) { m_view_project_matrix = vp; }
		void setLightingEnable(bool enable) { m_lighting_enable = enable; }

		//Fragment shader setting
		void setAmbientCoef(const glm::vec3 &ka) { m_ka = ka; }
		void setDiffuseCoef(const glm::vec3 &kd) { m_kd = kd; }
		void setSpecularCoef(const glm::vec3 &ks) { m_ks = ks; }
		void setEmissionColor(const glm::vec3 &ke) { m_ke = ke; }
		void setDiffuseTexId(const int &id) { m_diffuse_tex_id = id; }
		void setSpecularTexId(const int &id) { m_specular_tex_id = id; }
		void setNormalTexId(const int &id) { m_normal_tex_id = id; }
		void setGlowTexId(const int &id) { m_glow_tex_id = id; }
		void setShininess(const float &shininess) { m_shininess = shininess; }
		void setTangent(const glm::vec3 &tangent) { m_tangent = tangent; }
		void setBitangent(const glm::vec3 &bitangent) { m_bitangent = bitangent; }

		//Shaders
		virtual void vertexShader(VertexData &vertex) = 0;
		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) = 0;

		//Rasterization
		static void rasterize_wire(
			const VertexData &v0,
			const VertexData &v1,
			const VertexData &v2,
			const unsigned int &screen_width,
			const unsigned int &screene_height, 
			std::vector<VertexData> &rasterized_points);
		static void rasterize_fill_edge_function(
			const VertexData &v0,
			const VertexData &v1,
			const VertexData &v2,
			const unsigned int &screen_width,
			const unsigned int &screene_height,
			std::vector<VertexData> &rasterized_points);

		//Textures and lights
		static int upload_texture_2D(TRTexture2D::ptr tex);
		static TRTexture2D::ptr getTexture2D(int index);
		static int addPointLight(glm::vec3 pos, glm::vec3 atten, glm::vec3 color);
		static TRPointLight& getPointLight(int index);
		//Task5
		static int addSpotLight(glm::vec3 pos, glm::vec3 dir, float cutOff, float outerCutOff);
		static TRSpotLight& getSpotLight(int index);

		
		static void setViewerPos(const glm::vec3 &viewer) { m_viewer_pos = viewer; }
		static glm::vec4 texture2D(const unsigned int &id, const glm::vec2 &uv);

	protected:

		//Auxiliary function
		static void rasterize_wire_aux(
			const VertexData &begin,
			const VertexData &end,
			const unsigned int &screen_width,
			const unsigned int &screene_height,
			std::vector<VertexData> &rasterized_points);

		glm::mat4 m_model_matrix = glm::mat4(1.0f);
		glm::mat3 m_inv_trans_model_matrix = glm::mat3(1.0f);
		glm::mat4 m_view_project_matrix = glm::mat4(1.0f);

		//Global shading setttings
		static std::vector<TRTexture2D::ptr> m_global_texture_units;
		static std::vector<TRPointLight> m_point_lights;
		//Task5
		static std::vector<TRSpotLight> m_spot_lights;
		static glm::vec3 m_viewer_pos;

		//Material setting
		glm::vec3 m_ka = glm::vec3(0.0f);
		glm::vec3 m_kd = glm::vec3(1.0f);
		glm::vec3 m_ks = glm::vec3(0.0f);
		glm::vec3 m_ke = glm::vec3(0.0f);
		float m_shininess = 0.0f;
		int m_diffuse_tex_id = -1;
		int m_specular_tex_id = -1;
		int m_normal_tex_id = -1;
		int m_glow_tex_id = -1;

		bool m_lighting_enable = true;

		glm::vec3 m_tangent;
		glm::vec3 m_bitangent;
	};

	class TRDefaultShadingPipeline : public TRShadingPipeline
	{
	public:

		typedef std::shared_ptr<TRDefaultShadingPipeline> ptr;

		virtual ~TRDefaultShadingPipeline() = default;

		virtual void vertexShader(VertexData &vertex) override;
		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) override;

	};

	class TRTextureShadingPipeline final : public TRDefaultShadingPipeline
	{
	public:

		typedef std::shared_ptr<TRTextureShadingPipeline> ptr;

		virtual ~TRTextureShadingPipeline() = default;

		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) override;
	};

	class TRPhongShadingPipeline final : public TRDefaultShadingPipeline
	{
	public:
		typedef std::shared_ptr<TRPhongShadingPipeline> ptr;

		virtual ~TRPhongShadingPipeline() = default;

		virtual void fragmentShader(const VertexData &data, glm::vec4 &fragColor) override;

	private:
		void fetchFragmentColor(glm::vec3 &amb, glm::vec3 &diff, glm::vec3 &spec, const glm::vec2 &uv) const;
	};
}

#endif