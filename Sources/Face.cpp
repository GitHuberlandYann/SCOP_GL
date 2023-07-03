#include "scop.h"

Face::Face( Material *mat ) : _size(0), _texture_index(NULL)
{
	if (mat) {
		_color = mat->get_color();
		_texture_index = mat->get_texture_index();
	} else {
		set_vertex(_color, 1.0f, 0.0f, 0.0f);
	}
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

void Face::add_vertex_array(GLfloat *vertices, size_t & index, t_vertex vert, t_vertex vert_tex, t_vertex vert_nor)
{
	// if (index < 50)
	// {
	// 	std::cout << "current value of index: " << index << std::endl;
	// 	std::cout << "\tvert " << vert.x << ", " << vert.y << ", " << vert.z << std::endl;
	// 	std::cout << "\tcol " << _color.x << ", " << _color.y << ", " << _color.z << std::endl;
	// 	std::cout << "\tvert_tex " << vert_tex.x << ", " << vert_tex.y << std::endl;
	// 	std::cout << "\tvert_nor " << vert_nor.x << ", " << vert_nor.y << ", " << vert_nor.z << std::endl;
	// }
	vertices[index] = vert.x;
	vertices[index + 1] = vert.y;
	vertices[index + 2] = vert.z;
	vertices[index + 3] = _color.x;
	vertices[index + 4] = _color.y;
	vertices[index + 5] = _color.z;
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
				add_vertex_array(vertices, index, vs[_vertices[0]], default_texture, default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_texture, default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_texture, default_normal);
				break ;
			}
			case ONLY_VT:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], vts[_vertices_textures[0]], default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], vts[_vertices_textures[vindex]], default_normal);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], vts[_vertices_textures[vindex + 1]], default_normal);
				break ;
			}
			case ONLY_VN:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], default_texture, vns[_vertices_normals[0]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], default_texture, vns[_vertices_normals[vindex]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], default_texture, vns[_vertices_normals[vindex + 1]]);
				break ;
			}
			case VTN:
			{
				add_vertex_array(vertices, index, vs[_vertices[0]], vts[_vertices_textures[0]], vns[_vertices_normals[0]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex]], vts[_vertices_textures[vindex]], vns[_vertices_normals[vindex]]);
				add_vertex_array(vertices, index, vs[_vertices[vindex + 1]], vts[_vertices_textures[vindex + 1]], vns[_vertices_normals[vindex + 1]]);
				break ;
			}
		}
	}

}
