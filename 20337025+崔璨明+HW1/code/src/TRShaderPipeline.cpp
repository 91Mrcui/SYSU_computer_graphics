#include "TRShaderPipeline.h"

namespace TinyRenderer
{

	//----------------------------------------------VertexData----------------------------------------------

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::lerp(
		const TRShaderPipeline::VertexData &v0,
		const TRShaderPipeline::VertexData &v1,
		float frac)
	{
		//Linear interpolation
		VertexData result;
		result.pos = (1.0f - frac) * v0.pos + frac * v1.pos;
		result.col = (1.0f - frac) * v0.col + frac * v1.col;
		result.nor = (1.0f - frac) * v0.nor + frac * v1.nor;
		result.tex = (1.0f - frac) * v0.tex + frac * v1.tex;
		result.cpos = (1.0f - frac) * v0.cpos + frac * v1.cpos;
		result.spos.x = (1.0f - frac) * v0.spos.x + frac * v1.spos.x;
		result.spos.y = (1.0f - frac) * v0.spos.y + frac * v1.spos.y;

		return result;
	}

	void TRShaderPipeline::VertexData::prePerspCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by 1/w before rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		v.pos.w = 1.0f / v.cpos.w;
		v.pos = glm::vec4(v.pos.x * v.pos.w, v.pos.y * v.pos.w, v.pos.z * v.pos.w, v.pos.w);
		v.tex = v.tex * v.pos.w;
		v.nor = v.nor * v.pos.w;
		v.col = v.col * v.pos.w;
	}

	void TRShaderPipeline::VertexData::aftPrespCorrection(VertexData &v)
	{
		//Perspective correction: the world space properties should be multipy by w after rasterization
		//https://zhuanlan.zhihu.com/p/144331875
		//We use pos.w to store 1/w
		float w = 1.0f / v.pos.w;
		v.pos = v.pos * w;
		v.tex = v.tex * w;
		v.nor = v.nor * w;
		v.col = v.col * w;
	}

	//----------------------------------------------TRShaderPipeline----------------------------------------------

	std::vector<TRShaderPipeline::VertexData> TRShaderPipeline::rasterize_wire(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screen_height)
	{
		std::vector<VertexData> rasterized_points;
		auto line1 = rasterize_wire_aux(v0, v1, screen_width, screen_height);
		auto line2 = rasterize_wire_aux(v1, v2, screen_width, screen_height);
		auto line3 = rasterize_wire_aux(v0, v2, screen_width, screen_height);

		rasterized_points.insert(rasterized_points.end(), line1.begin(), line1.end());
		rasterized_points.insert(rasterized_points.end(), line2.begin(), line2.end());
		rasterized_points.insert(rasterized_points.end(), line3.begin(), line3.end());
		
		return rasterized_points;
	}

	std::vector<TRShaderPipeline::VertexData> TRShaderPipeline::rasterize_fill(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screen_height)
	{
		return {};
	}

	std::vector<TRShaderPipeline::VertexData> TRShaderPipeline::rasterize_wire_aux(
		const VertexData &from,
		const VertexData &to,
		const unsigned int &screen_width,
		const unsigned int &screen_height)
	{
		//Bresenham line rasterization
		std::vector<VertexData> rasterized_points;

		int dx = to.spos.x - from.spos.x;
		int dy = to.spos.y - from.spos.y;
		int stepX = 1, stepY = 1;

		// judge the sign
		if (dx < 0)
		{
			stepX = -1;
			dx = -dx;
		}
		if (dy < 0)
		{
			stepY = -1;
			dy = -dy;
		}

		int d2x = 2 * dx, d2y = 2 * dy;
		int d2y_minus_d2x = d2y - d2x;
		int sx = from.spos.x;
		int sy = from.spos.y;

		// slope < 1.
		if (dy <= dx)
		{
			int flag = d2y - dx;
			for (int i = 0; i <= dx; ++i)
			{
				auto mid = VertexData::lerp(from, to, static_cast<float>(i) / dx);
				mid.spos = glm::vec4(sx, sy, 0.0f, 1.0f);
				if (mid.spos.x >= 0 && mid.spos.x < screen_width && mid.spos.y >= 0 && mid.spos.y < screen_height)
				{
					rasterized_points.push_back(mid);
				}
				sx += stepX;
				if (flag <= 0)
				{
					flag += d2y;
				}
				else
				{
					sy += stepY;
					flag += d2y_minus_d2x;
				}
			}
		}
		// slope > 1.
		else
		{
			int flag = d2x - dy;
			for (int i = 0; i <= dy; ++i)
			{
				auto mid = VertexData::lerp(from, to, static_cast<float>(i) / dy);
				mid.spos = glm::vec4(sx, sy, 0.0f, 1.0f);
				if (mid.spos.x >= 0 && mid.spos.x < screen_width && mid.spos.y >= 0 && mid.spos.y < screen_height)
				{
					rasterized_points.push_back(mid);
				}
				sy += stepY;
				if (flag <= 0)
				{
					flag += d2x;
				}
				else
				{
					sx += stepX;
					flag -= d2y_minus_d2x;
				}
			}
		}

		return rasterized_points;
	}

	void TRDefaultShaderPipeline::vertexShader(VertexData &vertex)
	{
		//Local space -> World space -> Camera space -> Clip space
		vertex.pos = m_model_matrix * glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f);
		vertex.cpos = m_view_project_matrix * vertex.pos;
	}

	void TRDefaultShaderPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Just return the color.
		fragColor = glm::vec4(data.nor, 1.0f);
		fragColor = glm::vec4(data.tex, 0.0f, 1.0f);
	}
}