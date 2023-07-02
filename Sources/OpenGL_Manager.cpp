# include "scop.h"

OpenGL_Manager::OpenGL_Manager( void ) : _window(NULL)
{
	std::cout << "Constructor of OpenGL_Manager called" << std::endl;

}

OpenGL_Manager::~OpenGL_Manager( void )
{
	std::cout << "Destructor of OpenGL_Manager called" << std::endl;
	glfwMakeContextCurrent(NULL);
    glfwTerminate();
}

// ************************************************************************** //
//                                Private                                     //
// ************************************************************************** //

void OpenGL_Manager::check_glstate( std::string str )
{
	GLenum error_check = glGetError();	
	if (error_check) {
		std::cerr << "glGetError set to " << error_check << ", quitting now" << std::endl;
		exit(1);
	}
	std::cout << str << std::endl;
}

// ************************************************************************** //
//                                Public                                      //
// ************************************************************************** //

void OpenGL_Manager::setup_window( void )
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(1200, 900, "OpenGL", nullptr, nullptr);

	// activate opengl context
	glfwMakeContextCurrent(_window);

	// glew is there to use the correct version for all functions
	glewExperimental = GL_TRUE;
	glewInit();
}

void OpenGL_Manager::main_loop( void )
{
	check_glstate( "setup done, entering main loop" );

	// main loop cheking for inputs and rendering everything
	while(!glfwWindowShouldClose(_window))
	{
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, GL_TRUE);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}
