#ifndef OPENGL_MANAGER_HPP
# define OPENGL_MANAGER_HPP

# define WIN_WIDTH 1200
# define WIN_HEIGHT 900

class OpenGL_Manager
{
	private:
		GLFWwindow* _window;

		void check_glstate( std::string str );

	public:
		OpenGL_Manager( void );
		~OpenGL_Manager( void );

		void setup_window( void );
		// void setup_shaders( void );
		// void setup_object( void );
		void main_loop( void );
};

#endif
