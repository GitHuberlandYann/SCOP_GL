#ifndef FACE_HPP
# define FACE_HPP

# include "scop.h"

class Face
{
	private:
		size_t _size;
		std::vector<unsigned int> _vertices;
		std::vector<unsigned int> _vertices_textures;
		std::vector<unsigned int> _vertices_normals;

		void add_vertex_array(GLfloat *vertices, size_t & index, t_vertex vert, t_vertex col, t_vertex vert_tex, t_vertex vert_nor);
	public:
		Face( void );
		~Face( void );

		void add_vertex(int v_num, int vt_num, int vn_num);
		size_t get_size( void );
		void fill_vertex_array(GLfloat *vertices, size_t & index, t_face_mode face_mode,
							std::vector<t_vertex> vs, std::vector<t_vertex> vts, std::vector<t_vertex> vns);
};

#endif
