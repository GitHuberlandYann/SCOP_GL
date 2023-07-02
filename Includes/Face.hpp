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

	public:
		Face( void );
		~Face( void );

		void add_vertex(int v_num, int vt_num, int vn_num);
		size_t get_size( void );
};

#endif
