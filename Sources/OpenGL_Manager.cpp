# include "scop.h"

OpenGL_Manager::OpenGL_Manager( void ) : _window(NULL), _key_fill(0), _fill(GL_TRUE)
{
	std::cout << "Constructor of OpenGL_Manager called" << std::endl;

}

OpenGL_Manager::~OpenGL_Manager( void )
{
	std::cout << "Destructor of OpenGL_Manager called" << std::endl;
	glDeleteProgram(_shaderProgram);
    glDeleteShader(_fragmentShader);
    glDeleteShader(_vertexShader);

	glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

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

void OpenGL_Manager::compile_shader( GLuint ptrShader, std::string name )
{
	glCompileShader(ptrShader);

    GLint status;
    glGetShaderiv(ptrShader, GL_COMPILE_STATUS, &status);
	if (status) {
		std::cout << name << " shader compiled successfully" << std::endl;
	} else {
		char buffer[512];
		glGetShaderInfoLog(ptrShader, 512, NULL, buffer);

		std::cerr << name << " shader did not compile, error log:" << std::endl << buffer << std::endl;
		exit(1);
	}
}

void OpenGL_Manager::user_inputs( void )
{
		if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(_window, GL_TRUE);
		if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_PRESS) {
			if (glIsEnabled(GL_DEPTH_TEST))
				glDisable(GL_DEPTH_TEST);
			else
				glEnable(GL_DEPTH_TEST);
		}
		if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_PRESS && ++_key_fill == 1) {
			if (_fill)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			_fill = !_fill;
		} else if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_RELEASE)
			_key_fill = 0;
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

	_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "OpenGL", nullptr, nullptr);

	// activate opengl context
	glfwMakeContextCurrent(_window);

	// glew is there to use the correct version for all functions
	glewExperimental = GL_TRUE;
	glewInit();
	
	glEnable(GL_DEPTH_TEST);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	check_glstate("Window successfully created");
}

void OpenGL_Manager::setup_array_buffer( Parser *parser )
{
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	_number_vertices = parser->get_number_vertices();
	GLfloat *vertices = new GLfloat[_number_vertices * 11]; // X Y Z, R G B, U V, nX nY nZ
	std::cout << "total alloc of vertices: " << _number_vertices * 11 << std::endl;
	parser->fill_vertex_array(vertices);

	std::cout << "first vert: " << vertices[0] << ", " << vertices[1] << ", " << vertices[2] << std::endl;
	std::cout << "second vert: " << vertices[0 + 11] << ", " << vertices[1 + 11] << ", " << vertices[2 + 11] << std::endl;
	std::cout << "third vert: " << vertices[0 + 22] << ", " << vertices[1 + 22] << ", " << vertices[2 + 22] << std::endl;

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	check_glstate("Vertex buffer successfully created");
}

void OpenGL_Manager::create_shaders( void )
{
	std::string vertex_shader_data = get_file_content("Sources/Shaders/vertex.glsl");
	char *vertexSource = &vertex_shader_data[0];

	_vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(_vertexShader, 1, &vertexSource, NULL);
	compile_shader(_vertexShader, "vertex");

	std::string fragment_shader_data = get_file_content("Sources/Shaders/fragment.glsl");
	char *fragmentSource = &fragment_shader_data[0];

	_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(_fragmentShader, 1, &fragmentSource, NULL);
	compile_shader(_fragmentShader, "fragment");

	// Combining shaders into a program
	_shaderProgram = glCreateProgram();
	glAttachShader(_shaderProgram, _vertexShader);
	glAttachShader(_shaderProgram, _fragmentShader);

	glBindFragDataLocation(_shaderProgram, 0, "outColor");

	glLinkProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	check_glstate("Shader program successfully created");
}

void OpenGL_Manager::setup_shaders( void )
{
	GLint posAttrib = glGetAttribLocation(_shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0); 
}

void OpenGL_Manager::main_loop( void )
{
	check_glstate("setup done, entering main loop");

	std::cout << "number of vertices: " << _number_vertices << std::endl;

	// main loop cheking for inputs and rendering everything
	while (!glfwWindowShouldClose(_window))
	{
		user_inputs();
		
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // guess this can be outside while loop
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, _number_vertices);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	check_glstate("main loop successfully exited");
}
