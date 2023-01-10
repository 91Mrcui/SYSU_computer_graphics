#include "TRDrawableMesh.h"

#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace TinyRenderer
{

	void TRDrawableMesh::clear()
	{
		m_vertices_attrib.clear();
		std::vector<TRMeshFace>().swap(m_mesh_faces);
	}

	void TRDrawableMesh::loadMeshFromFile(const std::string &filename)
	{
		unload();

		tinyobj::ObjReaderConfig reader_config;

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(filename, reader_config)) 
		{
			if (!reader.Error().empty()) 
			{
				std::cerr << "TinyObjReader: " << reader.Error();
			}
			exit(1);
		}

		if (!reader.Warning().empty()) 
		{
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();

		for (size_t i = 0; i < attrib.vertices.size(); i += 3)
		{
			m_vertices_attrib.vpositions.push_back(
				glm::vec4(attrib.vertices[i + 0], attrib.vertices[i + 1], attrib.vertices[i + 2], 1.0f));
			m_vertices_attrib.vcolors.push_back(
				glm::vec4(attrib.colors[i + 0], attrib.colors[i + 1], attrib.colors[i + 2], 1.0f));
		}
		for (size_t i = 0; i < attrib.normals.size(); i += 3)
		{
			m_vertices_attrib.vnormals.push_back(
				glm::vec3(attrib.normals[i + 0], attrib.normals[i + 1], attrib.normals[i + 2]));
		}
		for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
		{
			m_vertices_attrib.vtexcoords.push_back(
				glm::vec2(attrib.texcoords[i + 0], attrib.texcoords[i + 1]));
		}
		for (size_t s = 0; s < shapes.size(); ++s)
		{
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f)
			{
				int fv = shapes[s].mesh.num_face_vertices[f];
				TRMeshFace face;
				for (size_t v = 0; v < fv && v < 3; ++v)
				{
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					face.vposIndex[v] = idx.vertex_index;
					face.vnorIndex[v] = idx.normal_index;
					face.vtexIndex[v] = idx.texcoord_index;
				}
				m_mesh_faces.push_back(face);
				index_offset += fv;
			}
		}
		
	}

	void TRDrawableMesh::unload()
	{
		clear();
	}
}