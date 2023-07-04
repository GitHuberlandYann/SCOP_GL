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

typedef struct {
	unsigned char *texture;
	int width;
	int height;
}				t_tex;

class Face;
class Material;

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
		std::vector<Material *> _materials;
		std::vector<t_tex *> _textures;
		std::vector<std::pair<int, size_t *> > _nbvert_index_textures;
		Material *_current_used_material;
		unsigned int _number_vertices;

		void push_vertex( t_vertex vertex );
		int check_vindex( int num );
		int check_vtindex( int num );
		int check_vnindex( int num );
		void add_vertex_face( Face *face, std::string line, size_t & index );
		void push_face( std::string line );
		void add_materials( std::string file );
		void set_material( std::string name );
		float get_extremum( void );
		int check_duplicate_xpm( std::vector<Material *>::iterator it, std::vector<Material *>::iterator ite );

	public:
		Parser( std::string root );
		~Parser( void );

		void parse( std::string file );
		void load_textures( void );
		void center_object( void );
		void display_content( void );

		unsigned int get_number_vertices( void );
		int get_number_textures( void );
		std::vector<t_tex *> get_textures( void );
		std::vector<std::pair<int, size_t *> > get_nbvert_index_textures( void );
		void fill_vertex_array(GLfloat *vertices);
};

#endif
