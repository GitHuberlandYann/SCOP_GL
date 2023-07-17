#ifndef FACE_HPP
# define FACE_HPP

# include "scop.h"

class Material;

class Face
{
	private:
		size_t _size;
		std::vector<t_vertex *> _vertices;
		std::vector<t_vertex *> _vertices_textures;
		std::vector<t_vertex *> _vertices_normals;
		t_vertex _color;
		size_t *_texture_index;

		void add_vertex_array( GLfloat *vertices, size_t & index, t_vertex *vert, t_vertex *vert_tex, t_vertex *vert_nor );
	public:
		Face( Material *mat );
		~Face( void );

		void add_vertex( t_vertex *v, t_vertex *vt, t_vertex *vn );
		size_t get_size( void );
		void fill_vertex_array( GLfloat *vertices, size_t & index );
};

#endif
