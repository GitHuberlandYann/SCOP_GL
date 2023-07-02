#ifndef OPENGL_MANAGER_HPP
# define OPENGL_MANAGER_HPP

# define WIN_WIDTH 1200
# define WIN_HEIGHT 900

class Parser;

class OpenGL_Manager
{
	private:
		GLFWwindow* _window;
		GLuint _vao, _vbo; // vertex array objects, vertex buffer objects
		GLuint _shaderProgram, _vertexShader, _fragmentShader;
		GLuint _number_vertices;
		GLint _key_fill;
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
		void setup_shaders( void );
		void main_loop( void );
};

#endif
