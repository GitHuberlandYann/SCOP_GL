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
	// TEXTURE,
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
		GLint _uniColorMode, _uniModel, _uniScale;
		t_vertex _rotation;
		GLfloat _rotation_speed;
		GLuint _number_vertices;
		GLint _key_fill, _key_depth, _color_mode, _key_color_mode;
		GLfloat _zoom;
		GLboolean _fill;

		void check_glstate( std::string str );
		void compile_shader( GLuint ptrShader, std::string name );
		void user_inputs( void );

	public:
		OpenGL_Manager( void );
		~OpenGL_Manager( void );

		void setup_window( void );
		void setup_array_buffer( Parser *parser );
		void create_shaders( void );
		void setup_communication_shaders( void );
		void main_loop( void );
};

#endif
