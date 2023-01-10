#ifndef TRDRAWABLEMESH_H
#define TRDRAWABLEMESH_H

#include <vector>
#include <memory>
#include <string>

#include "glm/glm.hpp"

namespace TinyRenderer
{
	class TRVertexAttrib final
	{
	public:
		std::vector<glm::vec4> vpositions;
		std::vector<glm::vec4> vcolors;
		std::vector<glm::vec2> vtexcoords;
		std::vector<glm::vec3> vnormals;

		void clear()
		{
			std::vector<glm::vec4>().swap(vpositions);
			std::vector<glm::vec4>().swap(vcolors);
			std::vector<glm::vec2>().swap(vtexcoords);
			std::vector<glm::vec3>().swap(vnormals);

		}
	};

	class TRMeshFace final
	{
	public:
		unsigned int vposIndex[3];
		unsigned int vnorIndex[3];
		unsigned int vtexIndex[3];
	};

	class TRDrawableMesh final
	{
	public:

		typedef std::shared_ptr<TRDrawableMesh> ptr;

		TRDrawableMesh() = default;
		~TRDrawableMesh() = default;
		
		TRDrawableMesh(const TRDrawableMesh& mesh)
			: m_vertices_attrib(mesh.m_vertices_attrib), m_mesh_faces(mesh.m_mesh_faces) {}

		TRDrawableMesh& operator=(const TRDrawableMesh& mesh)
		{
			if (&mesh == this)
				return *this;
			m_vertices_attrib = mesh.m_vertices_attrib;
			m_mesh_faces = mesh.m_mesh_faces;
			return *this;
		}

		void loadMeshFromFile(const std::string &filename);

		const TRVertexAttrib& getVerticesAttrib() const { return m_vertices_attrib; }
		const std::vector<TRMeshFace>& getMeshFaces() const { return m_mesh_faces; }

		void clear();
		void unload();

	private:
		TRVertexAttrib m_vertices_attrib;
		std::vector<TRMeshFace> m_mesh_faces;
	};

}

#endif