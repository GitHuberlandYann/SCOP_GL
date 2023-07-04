#ifndef OPENGL_MANAGER_HPP
# define OPENGL_MANAGER_HPP

# if __linux__
#  define WIN_WIDTH 1800
#  define WIN_HEIGHT 900
# else
#  define WIN_WIDTH 2560
#  define WIN_HEIGHT 1400
# endif

enum {
	DEFAULT,
	MATERIAL,
	TEXTURE,
	// GRAY,
	GRADIENT,
	LAST
};

class Parser;

class OpenGL_Manager
{
	private:
		GLFWwindow* _window;
		GLuint _vao, _vbo; // vertex array objects, vertex buffer objects
		GLuint _shaderProgram, _vertexShader, _fragmentShader;
		GLint _uniColorMode, _uniTexIndex, _uniModel, _uniScale, _nb_textures, _number_vertices;
		GLuint *_textures;
		t_vertex _rotation, _background_color;
		GLfloat _rotation_speed, _zoom;
		GLint _key_fill, _key_depth, _color_mode, _key_color_mode;
		GLboolean _fill;
		std::vector<std::pair<int, int> > _vert_tex_pair;
		size_t _vtp_size;

		void check_glstate( std::string str );
		void compile_shader( GLuint ptrShader, std::string name );
		std::string create_fragment_shader( std::string data );
		void user_inputs( void );

	public:
		OpenGL_Manager( GLint nb_textures, std::vector<std::pair<int, size_t *> > vert_tex_pair );
		~OpenGL_Manager( void );

		void setup_window( void );
		void setup_array_buffer( Parser *parser );
		void create_shaders( void );
		void setup_communication_shaders( void );
		void load_textures( Parser *parser );
		void main_loop( void );
};

#endif
