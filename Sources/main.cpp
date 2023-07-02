#include "scop.h"

int main( void )
{
	std::cout << " ---- Hello World ----" << std::endl;

	glfwInit();
    // std::this_thread::sleep_for(std::chrono::seconds(1));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1200, 900, "OpenGL", nullptr, nullptr);

	// activate opengl context
	glfwMakeContextCurrent(window);

	// glew is there to use the correct version for all functions
	glewExperimental = GL_TRUE;
	glewInit();

	GLenum error_check = glGetError();	
	if (error_check) {
		std::cerr << "glGetError set to " << error_check << ", quitting now" << std::endl;
		return (1);
	} else {
		std::cout << "setup done, entering main loop" << std::endl;
	}

	// main loop cheking for inputs and rendering everything
	while(!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwMakeContextCurrent(NULL);
    glfwTerminate();

	std::cout << " ---- Goodbye World ----" << std::endl;
	return (0);
}
