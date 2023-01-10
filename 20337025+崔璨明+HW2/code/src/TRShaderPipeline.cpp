#include "TRShaderPipeline.h"

#include <algorithm>

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

	TRShaderPipeline::VertexData TRShaderPipeline::VertexData::barycentricLerp(
		const VertexData &v0, 
		const VertexData &v1, 
		const VertexData &v2,
		glm::vec3 w)
	{
		VertexData result;
		result.pos = w.x * v0.pos + w.y * v1.pos + w.z * v2.pos;
		result.col = w.x * v0.col + w.y * v1.col + w.z * v2.col;
		result.nor = w.x * v0.nor + w.y * v1.nor + w.z * v2.nor;
		result.tex = w.x * v0.tex + w.y * v1.tex + w.z * v2.tex;
		result.cpos = w.x * v0.cpos + w.y * v1.cpos + w.z * v2.cpos;
		result.spos.x = w.x * v0.spos.x + w.y * v1.spos.x + w.z * v2.spos.x;
		result.spos.y = w.x * v0.spos.y + w.y * v1.spos.y + w.z * v2.spos.y;

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

	void TRShaderPipeline::rasterize_wire(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screene_height,
		std::vector<VertexData> &rasterized_points)
	{
		//Draw each line step by step
		rasterize_wire_aux(v0, v1, screen_width, screene_height, rasterized_points);
		rasterize_wire_aux(v1, v2, screen_width, screene_height, rasterized_points);
		rasterize_wire_aux(v0, v2, screen_width, screene_height, rasterized_points);
	}

	void TRShaderPipeline::rasterize_fill_edge_function(
		const VertexData &v0,
		const VertexData &v1,
		const VertexData &v2,
		const unsigned int &screen_width,
		const unsigned int &screene_height,
		std::vector<VertexData> &rasterized_points)
	{
		//Edge-function rasterization algorithm

		//Task4: Implement edge-function triangle rassterization algorithm
		// Note: You should use VertexData::barycentricLerp(v0, v1, v2, w) for interpolation, 
		//       interpolated points should be pushed back to rasterized_points.
		//       Interpolated points shold be discarded if they are outside the window. 

		//       v0.spos, v1.spos and v2.spos are the screen space vertices.

		//For instance:
		int max_X = std::max({ v0.spos.x, v1.spos.x, v2.spos.x });
		int max_Y = std::max({ v0.spos.y, v1.spos.y, v2.spos.y });
		int min_X = std::min({ v0.spos.x, v1.spos.x, v2.spos.x });
		int min_Y = std::min({ v0.spos.y, v1.spos.y, v2.spos.y });
		for (int i = min_X; i <= max_X; i++) {
			for (int j = min_Y; j <= max_Y; j++) {
				glm::vec3 x = glm::vec3(v1.spos.x - v0.spos.x, v2.spos.x - v0.spos.x, v0.spos.x - i);
				glm::vec3 y = glm::vec3(v1.spos.y - v0.spos.y, v2.spos.y - v0.spos.y, v0.spos.y - j);
				glm::vec3 n = glm::cross(x, y);//叉乘得到n
				glm::vec3 w = { 1.0f - n.x/n.z - n.y/n.z, n.x / n.z, n.y / n.z };
				//判断
				if (w.x >= 0 && w.y >= 0 && w.x + w.y <= 1) {
					VertexData interploated = VertexData::barycentricLerp(v0, v1, v2, w);
					interploated.spos.x = i, interploated.spos.y = j;
					rasterized_points.push_back(interploated);
				}
			}
		}

		//rasterized_points.push_back(v0);
		//rasterized_points.push_back(v1);
		//rasterized_points.push_back(v2);

	}

	void TRShaderPipeline::rasterize_wire_aux(
		const VertexData &from,
		const VertexData &to,
		const unsigned int &screen_width,
		const unsigned int &screen_height,
		std::vector<VertexData> &rasterized_points)
	{

		//Task1: Implement Bresenham line rasterization
		// Note: You shold use VertexData::lerp(from, to, weight) for interpolation,
		//       interpolated points should be pushed back to rasterized_points.
		//       Interpolated points shold be discarded if they are outside the window. 
		
		//       from.spos and to.spos are the screen space vertices.

		//For instance:
		int dx = to.spos.x - from.spos.x;
		int dy = to.spos.y - from.spos.y;
		int stepX = 1, stepY = 1;
		// 判断符号
		if (dx < 0) {
			stepX = -1;
			dx = -dx;
		}
		if (dy < 0) {
			stepY = -1;
			dy = -dy;
		}

		int d2x = 2 * dx, d2y = 2 * dy;
		int d2y_minus_d2x = d2y - d2x;
		int sx = from.spos.x;
		int sy = from.spos.y;

		// slope k <= 1
		if (dy <= dx) {
			int flag = d2y - dx;
			for (int i = 0; i <= dx; i++) {
				// linear interpolation
				VertexData tmp = VertexData::lerp(from, to, static_cast<double>(i) / dx);
				// 判断是否在屏幕内,位于窗口外，则丢弃这些插值点。
				if (tmp.spos.x >= 0 && tmp.spos.x < screen_width
					&& tmp.spos.y >= 0 && tmp.spos.y < screen_height) {
					rasterized_points.push_back(tmp);//存放插值点
				}
				sx += stepX;
				if (flag <= 0) {
					flag += d2y;
				}
				else {
					sy += stepY;
					flag += d2y_minus_d2x;
				}
			}
		}
		//slope k > 1
		else {
			int flag = d2x - dy;

			for (int i = 0; i <= dy; i++) {
				VertexData tmp = VertexData::lerp(from, to, static_cast<double>(i) / dy);
				if (tmp.spos.x >= 0 && tmp.spos.x < screen_width
					&& tmp.spos.y >= 0 && tmp.spos.y < screen_height) {
					rasterized_points.push_back(tmp);
				}
				sy += stepY;
				if (flag <= 0) {
					flag += d2x;
				}
				else {
					sx += stepX;
					flag += d2y_minus_d2x;
				}
			}
		}
		//rasterized_points.push_back(from);
		//rasterized_points.push_back(to);

	}

	void TRDefaultShaderPipeline::vertexShader(VertexData &vertex)
	{
		//Local space -> World space -> Camera space -> Project space
		vertex.pos = m_model_matrix * glm::vec4(vertex.pos.x, vertex.pos.y, vertex.pos.z, 1.0f);
		vertex.cpos = m_view_project_matrix * vertex.pos;
	}

	void TRDefaultShaderPipeline::fragmentShader(const VertexData &data, glm::vec4 &fragColor)
	{
		//Just return the color.
		fragColor = glm::vec4(data.tex, 0.0, 1.0f);
	}
}