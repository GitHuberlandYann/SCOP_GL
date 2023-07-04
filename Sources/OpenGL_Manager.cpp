# include "scop.h"

OpenGL_Manager::OpenGL_Manager( GLint nb_textures, std::vector<std::pair<int, size_t *> > vert_tex_pair )
	: _window(NULL), _nb_textures(nb_textures), _textures(NULL), _rotation_speed(1.5f), _zoom(1.0f),
		_key_fill(0), _key_depth(0), _color_mode(DEFAULT), _key_color_mode(0), _fill(GL_TRUE),
		_vtp_size(vert_tex_pair.size())
{
	std::cout << "Constructor of OpenGL_Manager called" << std::endl << std::endl;
	set_vertex(_rotation, 0.0f, 0.0f, 180.0f);
	set_vertex(_background_color, 0.5f, 0.5f, 0.5f);

	for (size_t index = 0; index < _vtp_size; index++) {
		size_t tex_index = *vert_tex_pair[index].second;
		int itex_index = -1;
		if (tex_index != std::string::npos)
			itex_index = (int)tex_index;
		_vert_tex_pair.push_back(std::pair<int, int>(vert_tex_pair[index].first, itex_index));
	}
}

OpenGL_Manager::~OpenGL_Manager( void )
{
	std::cout << "Destructor of OpenGL_Manager called" << std::endl;
	glDeleteTextures(_nb_textures, _textures);

	glDeleteProgram(_shaderProgram);
    glDeleteShader(_fragmentShader);
    glDeleteShader(_vertexShader);

	glDeleteBuffers(1, &_vbo);
    glDeleteVertexArrays(1, &_vao);

	glfwMakeContextCurrent(NULL);
    glfwTerminate();

	delete [] _textures;
	_vert_tex_pair.clear();
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

std::string OpenGL_Manager::create_fragment_shader( std::string data )
{
	std::string res;

	size_t tex_index = data.find("TEX"), out_index = data.find("OUT");
	if (tex_index == std::string::npos) {
		std::cerr << "no TEX in shader texture, please fix" << std::endl;
		return (res);
	} else if (out_index == std::string::npos) {
		std::cerr << "no OUT in shader texture, please fix" << std::endl;
		return (res);
	}
	
	res = data.substr(0, tex_index);
	if (_nb_textures) {
		res += "\r\n";
		for (int index = 0; index < _nb_textures; index++) {
			res += "uniform sampler2D tex" + std::to_string(index) + ";\r\n";
		}
	}

	res += data.substr(tex_index + 3, out_index - (tex_index + 3));
	if (_nb_textures) {
		res += "if (Tex_index[0] == 0 || Tex_index[1] == -1) {\r\n\t\toutColor = vec4(Color, 1.0);\r\n\t}";
		for (int index = 0; index < _nb_textures; index++) {
			std::string num = std::to_string(index);
			res += " else if (Tex_index[1] == " + num + ") {\r\n\t\toutColor = texture(tex" + num + ", Texcoord);\r\n\t}";
		}
		res += "\r\n";
	} else {
		res += "outColor = vec4(Color, 1.0);";
	}

	res += data.substr(out_index + 3);
	// display_special_characters(res);
	// std::cout << res;
	return (res);
}

void OpenGL_Manager::user_inputs( void )
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GL_TRUE);
	
	GLint key_rotation_z = (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS);
	GLint key_rotation_x = (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS);
	GLint key_rotation_y = (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS);
	if (key_rotation_z || key_rotation_y || key_rotation_x) {
		_rotation.x += key_rotation_x * _rotation_speed;
		_rotation.y += key_rotation_y * _rotation_speed;
		_rotation.z += key_rotation_z * _rotation_speed;
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));
	}

	GLint key_rotation_speed = (glfwGetKey(_window, GLFW_KEY_KP_ADD) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);
	if (key_rotation_speed && _rotation_speed + 0.1f * key_rotation_speed >= 0)
		_rotation_speed += 0.1f * key_rotation_speed;

	if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS && ++_key_color_mode == 1) {
		++_color_mode;
		if (_color_mode == LAST)
			_color_mode = DEFAULT;
		glUniform1i(_uniColorMode, _color_mode);
	} else if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_RELEASE)
		_key_color_mode = 0;

	if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_PRESS && ++_key_depth == 1) {
		if (glIsEnabled(GL_DEPTH_TEST))
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
	} else if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_RELEASE)
		_key_depth = 0;

	if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_PRESS && ++_key_fill == 1) {
		if (_fill)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		_fill = !_fill;
	} else if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_RELEASE)
		_key_fill = 0;
	
	GLint key_zoom = (glfwGetKey(_window, GLFW_KEY_EQUAL) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_MINUS) == GLFW_PRESS);
	if (key_zoom && _zoom + 0.1f * key_zoom >= 0) {
		_zoom += 0.1f * key_zoom;
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(_zoom));
		glUniformMatrix4fv(_uniScale, 1, GL_FALSE, glm::value_ptr(scale));
	}

	GLint key_col_r = (glfwGetKey(_window, GLFW_KEY_KP_7) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_4) == GLFW_PRESS);
	GLint key_col_g = (glfwGetKey(_window, GLFW_KEY_KP_8) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_5) == GLFW_PRESS);
	GLint key_col_b = (glfwGetKey(_window, GLFW_KEY_KP_9) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_6) == GLFW_PRESS);
	if (key_col_r || key_col_g || key_col_b) {
		_background_color.x = glm::clamp(_background_color.x + key_col_r * 0.01f, 0.f, 1.f);
		_background_color.y = glm::clamp(_background_color.y + key_col_g * 0.01f, 0.f, 1.f);
		_background_color.z = glm::clamp(_background_color.z + key_col_b * 0.01f, 0.f, 1.f);
		glClearColor(_background_color.x, _background_color.y, _background_color.z, 1.0f);
	}
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

	delete [] vertices;
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
	fragment_shader_data = create_fragment_shader(fragment_shader_data);
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

void OpenGL_Manager::setup_communication_shaders( void )
{
	// attributes
	GLint posAttrib = glGetAttribLocation(_shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), 0);

	GLint colAttrib = glGetAttribLocation(_shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

	GLint texAttrib = glGetAttribLocation(_shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (void *)(6 * sizeof(GLfloat)));

	// uniforms
	_uniColorMode = glGetUniformLocation(_shaderProgram, "color_mode");
	glUniform1i(_uniColorMode, _color_mode);
	
	_uniTexIndex = glGetUniformLocation(_shaderProgram, "tex_index");
	glUniform1i(_uniTexIndex, -1);

	_uniModel = glGetUniformLocation(_shaderProgram, "model");
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));

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
	glm::mat4 scale =  glm::scale(glm::mat4(1.0f), glm::vec3(_zoom));
	glUniformMatrix4fv(_uniScale, 1, GL_FALSE, glm::value_ptr(scale));
	
	check_glstate("Communication with shader program successfully established");
}

void OpenGL_Manager::load_textures( Parser *parser )
{
	if (!_nb_textures) {
		std::cout << "no texture to load on shader" << std::endl;
		return ;
	}

	std::vector<t_tex *> ui_textures = parser->get_textures();

	_textures = new GLuint[_nb_textures]; // malloc todo
	glGenTextures(_nb_textures, _textures);
	
	for (GLint index = 0; index < _nb_textures; index++)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, _textures[index]);

		// load image as texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui_textures[index]->width, ui_textures[index]->height,
					0, GL_RGB, GL_UNSIGNED_BYTE, ui_textures[index]->texture);

		std::string tex_str = "tex" + std::to_string(index);
		glUniform1i(glGetUniformLocation(_shaderProgram, tex_str.c_str()), index); // sampler2D #index in fragment shader
		
		// set settings for texture wraping and size modif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	std::string success_msg = "Succesfully loaded " + std::to_string(_nb_textures) + " to shader";
	check_glstate(success_msg);
}

void OpenGL_Manager::main_loop( void )
{
	check_glstate("setup done, entering main loop");

	std::cout << "number of vertices: " << _number_vertices << std::endl << std::endl;
	glClearColor(_background_color.x, _background_color.y, _background_color.z, 1.0f);

	// std::cout << "60fps game is 16.6666 ms/frame; 30fps game is 33.3333 ms/frame." << std::endl; 
	// double lastTime = glfwGetTime();
	// int nbFrames = 0;

	// main loop cheking for inputs and rendering everything
	while (!glfwWindowShouldClose(_window))
	{
		// double currentTime = glfwGetTime();
		// nbFrames++;
		// if ( currentTime - lastTime >= 1.0 ){
		// 	std::cout << 1000.0/double(nbFrames) << " ms/frame; " << nbFrames << " fps" << std::endl;
		// 	nbFrames = 0;
		// 	lastTime += 1.0;
		// }

		user_inputs();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!_nb_textures) {
			glDrawArrays(GL_TRIANGLES, 0, _number_vertices);
		} else {
			size_t start_index = 0;
			for (size_t index = 0; index < _vtp_size; index++) {
				glUniform1i(_uniTexIndex, _vert_tex_pair[index].second);

				glDrawArrays(GL_TRIANGLES, start_index, _vert_tex_pair[index].first);
				start_index += _vert_tex_pair[index].first;
			}
		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	std::cout << std::endl;
	check_glstate("main loop successfully exited");
}
