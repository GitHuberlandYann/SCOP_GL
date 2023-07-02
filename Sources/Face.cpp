#include "scop.h"

Face::Face( void ) : _size(0)
{

}

Face::~Face( void )
{
	_vertices.clear();
	_vertices_textures.clear();
	_vertices_normals.clear();
}


// ************************************************************************** //
//                                Private                                     //
// ************************************************************************** //

void Face::add_vertex_array(GLfloat *vertices, size_t & index, t_vertex vert, t_vertex col, t_vertex vert_tex, t_vertex vert_nor)
{
	// std::cout << "current value of index: " << index << std::endl; 46794
	vertices[index] = vert.x;
	vertices[index + 1] = vert.y;
	vertices[index + 2] = vert.z;
	vertices[index + 3] = col.x;
	vertices[index + 4] = col.y;
	vertices[index + 5] = col.z;
	vertices[index + 6] = vert_tex.x;
	vertices[index + 7] = vert_tex.y;
	vertices[index + 8] = vert_nor.x;
	vertices[index + 9] = vert_nor.y;
	vertices[index + 10] = vert_nor.z;
	index += 11;
}

// ************************************************************************** //
//                                Public                                      //
// ************************************************************************** //

void Face::add_vertex(int v_num, int vt_num, int vn_num)
{
	_vertices.push_back(v_num);
	if (vt_num != -1)
		_vertices_textures.push_back(vt_num);
	if (vn_num != -1)
		_vertices_normals.push_back(vn_num);

	++_size;
}

size_t Face::get_size( void )
{
	return (_size);
}

void Face::fill_vertex_array(GLfloat *vertices, size_t & index, t_face_mode face_mode,
							std::vector<t_vertex> vs, std::vector<t_vertex> vts, std::vector<t_vertex> vns)
{
	t_vertex default_color = {0.2f, 0.2f, 0.2f};
	t_vertex default_texture = {0.0f, 0.0f, 0.0f};
	t_vertex default_normal = {0.0f, 0.0f, 0.0f};

	for (size_t vindex = 1; vindex < _size - 1; ++vindex) {
		switch (face_mode) {
			case UNSET:
			{
				std::cerr << "ERROR UNSET MODE IN FACE FILL VERTEX ARRAY" << std::endl;
				exit(1);
			}
			case ONLY_V:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], default_color, default_texture, default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_color, default_texture, default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_color, default_texture, default_normal);
				break ;
			}
			case ONLY_VT:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], default_color, vts[_vertices_textures[0]], default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_color, vts[_vertices_textures[vindex]], default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_color, vts[_vertices_textures[vindex + 1]], default_normal);
				break ;
			}
			case ONLY_VN:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], default_color, default_texture, vns[_vertices_normals[0]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_color, default_texture, vns[_vertices_normals[vindex]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_color, default_texture, vns[_vertices_normals[vindex + 1]]);
				break ;
			}
			case VTN:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], default_color, vts[_vertices_textures[0]], vns[_vertices_normals[0]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_color, vts[_vertices_textures[vindex]], vns[_vertices_normals[vindex]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_color, vts[_vertices_textures[vindex + 1]], vns[_vertices_normals[vindex + 1]]);
				break ;
			}
		}
	}

}
