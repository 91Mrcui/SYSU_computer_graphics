#ifndef TRFRAMEBUFFER_H
#define TRFRAMEBUFFER_H

#include <vector>
#include <memory>

#include "glm/vec4.hpp"

namespace TinyRenderer
{
	class TRFrameBuffer final
	{
	public:
		typedef std::shared_ptr<TRFrameBuffer> ptr;

		// ctor/dtor.
		TRFrameBuffer(int width, int height);
		~TRFrameBuffer() = default;

		void clear(const glm::vec4 &color);

		// Getter.
		unsigned int getWidth()const { return m_width; }
		unsigned int getHeight()const { return m_height; }
		unsigned char *getColorBuffer() { return m_colorBuffer.data(); }

		double readDepth(const unsigned int &x, const unsigned int &y) const;
		void writeDepth(const unsigned int &x, const unsigned int &y, const double &value);
		void writeColor(const unsigned int &x, const unsigned int &y, const glm::vec4 &color);

	private:
		std::vector<double> m_depthBuffer;          // Z-buffer
		std::vector<unsigned char> m_colorBuffer;   // Color buffer
		unsigned int m_width, m_height, m_channel;  // Viewport
	};
}

#endif