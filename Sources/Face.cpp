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

void Face::add_vertex_array( GLfloat *vertices, size_t & index, t_vertex *vert, t_vertex *vert_tex, t_vertex *vert_nor )
{
	// if (index < 50)
	// {
	// 	std::cout << "current value of index: " << index << std::endl;
	// 	std::cout << "\tvert " << vert.x << ", " << vert.y << ", " << vert.z << std::endl;
	// 	std::cout << "\tcol " << _color.x << ", " << _color.y << ", " << _color.z << std::endl;
	// 	std::cout << "\tvert_tex " << vert_tex.x << ", " << vert_tex.y << std::endl;
	// 	std::cout << "\tvert_nor " << vert_nor.x << ", " << vert_nor.y << ", " << vert_nor.z << std::endl;
	// }
	// std::cout << "at index " << index << ", num vertex: " << (index / 36) % 3 << std::endl;
	vertices[index] = (index / 36) % 3;
	vertices[index + 1] = vert->x;
	vertices[index + 2] = vert->y;
	vertices[index + 3] = vert->z;
	vertices[index + 4] = _color.x;
	vertices[index + 5] = _color.y;
	vertices[index + 6] = _color.z;
	vertices[index + 7] = vert_tex->x;
	vertices[index + 8] = vert_tex->y;
	vertices[index + 9] = vert_nor->x;
	vertices[index + 10] = vert_nor->y;
	vertices[index + 11] = vert_nor->z;
	index += 12;
}

// ************************************************************************** //
//                                Public                                      //
// ************************************************************************** //

void Face::add_vertex( t_vertex *v, t_vertex *vt, t_vertex *vn )
{
	_vertices.push_back(v);
	_vertices_textures.push_back(vt);
	_vertices_normals.push_back(vn);

	++_size;
}

size_t Face::get_size( void )
{
	return (_size);
}

void Face::fill_vertex_array( GLfloat *vertices, size_t & index )
{
	for (size_t vindex = 1; vindex < _size - 1; ++vindex) {
				add_vertex_array(vertices, index, _vertices[0], _vertices_textures[0], _vertices_normals[0]);
				add_vertex_array(vertices, index, _vertices[vindex], _vertices_textures[vindex], _vertices_normals[vindex]);
				add_vertex_array(vertices, index, _vertices[vindex + 1], _vertices_textures[vindex + 1], _vertices_normals[vindex + 1]);
	}
}
