# include "scop.h"

OpenGL_Manager::OpenGL_Manager( void ) : _window(NULL), _rotation_offset(180.0f), _key_fill(0), _zoom(1.0f), _fill(GL_TRUE)
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
	if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
		++_rotation_offset;
	if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
		--_rotation_offset;
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
	
	if (glfwGetKey(_window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
		_zoom += 0.1;
	if (glfwGetKey(_window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
		_zoom -= 0.1;
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

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _number_vertices * 11 * sizeof(float), vertices, GL_STATIC_DRAW);

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

	if (posAttrib)
		std::cout << "posAttrib not at index 0, instead found at: " << posAttrib << std::endl;

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);

	_uniModel = glGetUniformLocation(_shaderProgram, "model");

	glm::mat4 view = glm::lookAt( // simulates a moving camera
		glm::vec3(2.5f, 2.5f, 2.0f), // world position of camera
		glm::vec3(0.0f, 0.0f, 0.0f), // point to be centered on screen
		glm::vec3(0.0f, 0.0f, 1.0f) // 'up' axis, here up is z axis, meaning x y is the ground
	);
	GLint uniView = glGetUniformLocation(_shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective( // perspective projection matrix
		glm::radians(45.0f),                          // vertical fov
		(GLfloat)WIN_WIDTH / (GLfloat)WIN_HEIGHT,     // aspect ratio of the screen
		1.0f,                                         // 'near' plane == clipping planes
		10.0f                                         // 'far' plane
	);
	GLint uniProj = glGetUniformLocation(_shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	_uniScale = glGetUniformLocation(_shaderProgram, "scale");
}

void OpenGL_Manager::main_loop( void )
{
	check_glstate("setup done, entering main loop");

	std::cout << "number of vertices: " << _number_vertices << std::endl;
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// main loop cheking for inputs and rendering everything
	while (!glfwWindowShouldClose(_window))
	{
		user_inputs();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(
			model,
			glm::radians(_rotation_offset),
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 scale =  glm::mat4(_zoom, 0.0f, 0.0f, 0.0f,
									0.0f, _zoom, 0.0f, 0.0f,
									0.0f, 0.0f, _zoom, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(_uniScale, 1, GL_FALSE, glm::value_ptr(scale));

		glDrawArrays(GL_TRIANGLES, 0, _number_vertices);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	check_glstate("main loop successfully exited");
}
