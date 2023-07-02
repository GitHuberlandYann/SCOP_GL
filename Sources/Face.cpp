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
