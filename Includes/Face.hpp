#ifndef FACE_HPP
# define FACE_HPP

# include "scop.h"

class Material;

class Face
{
	private:
		size_t _size;
		std::vector<unsigned int> _vertices;
		std::vector<unsigned int> _vertices_textures;
		std::vector<unsigned int> _vertices_normals;
		t_vertex _color;
		size_t *_texture_index;

		void add_vertex_array(GLfloat *vertices, size_t & index, t_vertex vert, t_vertex vert_tex, t_vertex vert_nor);
	public:
		Face( Material *mat );
		~Face( void );

		void add_vertex(int v_num, int vt_num, int vn_num);
		size_t get_size( void );
		void fill_vertex_array(GLfloat *vertices, size_t & index, t_face_mode face_mode,
							std::vector<t_vertex> vs, std::vector<t_vertex> vts, std::vector<t_vertex> vns);
};

#endif
