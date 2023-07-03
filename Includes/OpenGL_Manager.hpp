#ifndef OPENGL_MANAGER_HPP
# define OPENGL_MANAGER_HPP

# define WIN_WIDTH 2560
# define WIN_HEIGHT 1400

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
		GLfloat _rotation_offset;
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
