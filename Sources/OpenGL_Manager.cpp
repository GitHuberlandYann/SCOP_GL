# include "scop.h"

OpenGL_Manager::OpenGL_Manager( GLint nb_textures, std::vector<std::pair<int, size_t *> > vert_tex_pair, bool provided )
	: _window(NULL), _nb_textures(nb_textures + provided), _omore_tex(provided), _textures(NULL),
		_rotation_speed(1.5f), _zoom(1.0f), _point_size(1.0f), _key_fill(0), _fill(FILL), _key_depth(0),
		_color_mode(DEFAULT), _key_color_mode(0), _key_section(0), _invert_col(0), _key_invert(0),
		_use_light(0), _key_use_light(0), _mouse_x(0), _mouse_y(0), _vtp_size(vert_tex_pair.size())
{
	std::cout << "Constructor of OpenGL_Manager called" << std::endl << std::endl;
	set_vertex(_rotation, -90.0f, 0.0f, 0.0f);
	set_vertex(_background_color, 0.5f, 0.5f, 0.5f);
	_light_col = glm::vec3(1.0f, 1.0f, 1.0f);
	_light_angles = glm::vec2(20.0f, 40.0f);
	_light_pos = 2.0f * glm::vec3(glm::cos(glm::radians(_light_angles.x)) + glm::cos(glm::radians(_light_angles.y)),
								glm::sin(glm::radians(_light_angles.x)) + glm::cos(glm::radians(_light_angles.y)),
								glm::sin(glm::radians(_light_angles.y)));

	for (size_t index = 0; index < _vtp_size; index++) {
		size_t tex_index = *vert_tex_pair[index].second;
		int itex_index = -1;
		if (tex_index != std::string::npos)
			itex_index = (int)tex_index;
		_vert_tex_pair.push_back(std::pair<int, int>(vert_tex_pair[index].first, itex_index));
	}
	_section = _vtp_size;
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
		res += "if (UseTex == 0 || Tex_index == -1) {\r\n\t\toutColor = vec4(Color, 1.0);\r\n\t}";
		for (int index = 0; index < _nb_textures; index++) {
			std::string num = std::to_string(index);
			res += " else if (Tex_index == " + num + ") {\r\n\t\toutColor = texture(tex" + num + ", Texcoord);\r\n\t}";
		}
		res += " else { // not supposed to happen\r\n\t\toutColor = vec4(0.0, 1.0, 0.0, 1.0);\r\n\t}";
	} else {
		res += "outColor = vec4(Color, 1.0);";
	}

	res += data.substr(out_index + 3);
	// display_special_characters(res);
	// std::cout << res;
	std::ofstream savefile("Sources/Shaders/last_fragment.glsl");
	if (!savefile.is_open()) {
		std::cerr << "failed to open save file for fragment shader" << std::endl;
		return (res);
	}
	savefile << res;
	savefile.close();
	return (res);
}

// ************************************************************************** //
//                                Public                                      //
// ************************************************************************** //

void OpenGL_Manager::setup_window( std::string title )
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, title.c_str(), nullptr, nullptr);

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
	t_face_mode fm = parser->get_face_mode();
	_can_light = (fm == VTN) || (fm == ONLY_VN);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	_number_vertices = parser->get_number_vertices();

	GLfloat *vertices = new GLfloat[_number_vertices * 12]; // num, X Y Z, R G B, U V, nX nY nZ
	std::cout << "total alloc of vertices: " << _number_vertices * 12 << std::endl;
	parser->fill_vertex_array(vertices);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _number_vertices * 12 * sizeof(float), vertices, GL_STATIC_DRAW);

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

	glBindAttribLocation(_shaderProgram, NUMATTRIB, "face_num");
	glBindAttribLocation(_shaderProgram, POSATTRIB, "position");
	glBindAttribLocation(_shaderProgram, COLATTRIB, "color");
	glBindAttribLocation(_shaderProgram, TEXATTRIB, "texcoord");
	glBindAttribLocation(_shaderProgram, NORMATTRIB, "normal");

	glLinkProgram(_shaderProgram);
	glUseProgram(_shaderProgram);

	check_glstate("Shader program successfully created");
}

void OpenGL_Manager::setup_communication_shaders( void )
{
	// attributes
	glEnableVertexAttribArray(NUMATTRIB);
	glVertexAttribPointer(NUMATTRIB, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), 0);
	check_glstate("numAttrib successfully set");

	glEnableVertexAttribArray(POSATTRIB);
	glVertexAttribPointer(POSATTRIB, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(sizeof(GLfloat)));
	check_glstate("posAttrib successfully set");

	glEnableVertexAttribArray(COLATTRIB);
	glVertexAttribPointer(COLATTRIB, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(4 * sizeof(GLfloat)));
	check_glstate("colAttrib successfully set");

	glEnableVertexAttribArray(TEXATTRIB);
	glVertexAttribPointer(TEXATTRIB, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(7 * sizeof(GLfloat)));
	check_glstate("texAttrib successfully set");

	glEnableVertexAttribArray(NORMATTRIB);
	glVertexAttribPointer(NORMATTRIB, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(GLfloat), (void *)(9 * sizeof(GLfloat)));
	check_glstate("normAttrib successfully set");

	// uniforms
	_uniColorMode = glGetUniformLocation(_shaderProgram, "color_mode");
	glUniform1i(_uniColorMode, _color_mode);
	
	_uniTexIndex = glGetUniformLocation(_shaderProgram, "tex_index");
	glUniform1i(_uniTexIndex, -1);

	GLint uniLastTex = glGetUniformLocation(_shaderProgram, "last_tex");
	glUniform1i(uniLastTex, _nb_textures - 1 + !_nb_textures);

	_uniInvert = glGetUniformLocation(_shaderProgram, "invert_color");
	glUniform1i(_uniInvert, _invert_col);

	_uniUseLight = glGetUniformLocation(_shaderProgram, "use_light");
	glUniform1i(_uniUseLight, 0);

	_uniModel = glGetUniformLocation(_shaderProgram, "model");
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// _uniCamPos = glGetUniformLocation(_shaderProgram, "camPos");
	_uniView = glGetUniformLocation(_shaderProgram, "view");
	update_cam_view();

	_uniProj = glGetUniformLocation(_shaderProgram, "proj");
	update_cam_perspective();

	_uniScale = glGetUniformLocation(_shaderProgram, "scale");
	glm::mat4 scale =  glm::scale(glm::mat4(1.0f), glm::vec3(_zoom));
	glUniformMatrix4fv(_uniScale, 1, GL_FALSE, glm::value_ptr(scale));

	_uniLightPos = glGetUniformLocation(_shaderProgram, "lightPos");
	glUniform3fv(_uniLightPos, 1, glm::value_ptr(_light_pos));

	_uniLightColor = glGetUniformLocation(_shaderProgram, "lightColor");
	glUniform3fv(_uniLightColor, 1, glm::value_ptr(_light_col));

	check_glstate("Communication with shader program successfully established");
}

void OpenGL_Manager::load_textures( Parser *parser, t_tex *provided_tex )
{
	if (!_nb_textures) {
		std::cout << "no texture to load on shader" << std::endl;
		return ;
	}

	std::vector<t_tex *> ui_textures = parser->get_textures();
	if (provided_tex) {
		ui_textures.push_back(provided_tex);
	}

	_textures = new GLuint[_nb_textures];
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

	std::string success_msg = "Succesfully loaded " + std::to_string(_nb_textures) + " textures to shader";
	check_glstate(success_msg);
}

void OpenGL_Manager::main_loop( void )
{
	check_glstate("setup done, entering main loop");

	std::cout << "number of vertices: " << _number_vertices << std::endl << std::endl;
	
	glClearColor(_background_color.x, _background_color.y, _background_color.z, 1.0f);
	// glfwSetCursorPosCallback(_window, cursor_position_callback);
    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		if (!_vtp_size) {
			glDrawArrays(GL_TRIANGLES, 0, _number_vertices);
		} else if (_section == (int)_vtp_size) {
			for (size_t index = 0, start_index = 0; index < _vtp_size; index++) {
				glUniform1i(_uniTexIndex, _vert_tex_pair[index].second);

				glDrawArrays(GL_TRIANGLES, start_index, _vert_tex_pair[index].first);
				start_index += _vert_tex_pair[index].first;
			}
		} else {
			int start_index = 0;
			for (int index = 0; index < _section; index++) {
				start_index += _vert_tex_pair[index].first;
			}
			glUniform1i(_uniTexIndex, _vert_tex_pair[_section].second);
			glDrawArrays(GL_TRIANGLES, start_index, _vert_tex_pair[_section].first);
		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	check_glstate("main loop successfully exited");
}
