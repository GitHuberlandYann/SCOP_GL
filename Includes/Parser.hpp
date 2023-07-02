#ifndef PARSER_HPP
# define PARSER_HPP

# include "scop.h"

typedef enum {
	UNSET,
	ONLY_V,
	ONLY_VT,
	ONLY_VN,
	VTN
}		t_face_mode;

class Face;

class Parser
{
	private:
		std::string _root;
		t_face_mode _face_mode;
		t_vertex _max_box;
		t_vertex _min_box;
		std::vector<t_vertex> _vertices;
		std::vector<t_vertex> _vertices_textures;
		std::vector<t_vertex> _vertices_normals;
		std::vector<Face *> _faces;
		unsigned int _number_vertices;

		void push_vertex( t_vertex vertex );
		int check_vindex( int num );
		int check_vtindex( int num );
		int check_vnindex( int num );
		void add_vertex_face( Face *face, std::string line, size_t & index );
		void push_face( std::string line );
		float get_extremum( void );

	public:
		Parser( std::string root );
		~Parser( void );

		void parse( std::string file );
		void center_object( void );
		void display_content( void );
		unsigned int get_number_vertices( void );
		void fill_vertex_array(GLfloat *vertices);
};

#endif
