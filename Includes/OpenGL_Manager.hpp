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
	TEXTURE,
	PROVIDED,
	GRAY,
	GRADIENT,
	LAST
};

enum {
	NUMATTRIB,
	POSATTRIB,
	COLATTRIB,
	TEXATTRIB,
	NORMATTRIB
};

enum {
	FILL,
	LINE,
	POINT,
	F_LAST
};

class Parser;

class OpenGL_Manager
{
	private:
		GLFWwindow* _window;
		GLuint _vao, _vbo; // vertex array objects, vertex buffer objects
		GLuint _shaderProgram, _vertexShader, _fragmentShader;
		GLint _uniColorMode, _uniTexIndex, _uniInvert, _uniUseLight, _uniModel, _uniView, _uniProj, _uniScale, _uniLightPos, _uniLightColor;// , _uniCamPos
		GLint _nb_textures, _number_vertices, _section;
		bool _omore_tex, _can_light;
		GLuint *_textures;
		t_vertex _rotation, _background_color;
		glm::vec3 _light_pos, _light_col;
		glm::vec2 _light_angles;
		GLfloat _rotation_speed, _zoom, _point_size;
		GLint _key_fill, _fill, _key_depth, _color_mode, _key_color_mode, _key_section, _invert_col, _key_invert, _use_light, _key_use_light;
		GLdouble _mouse_x, _mouse_y;
		std::vector<std::pair<int, int> > _vert_tex_pair;
		size_t _vtp_size;

		void check_glstate( std::string str );
		void compile_shader( GLuint ptrShader, std::string name );
		std::string create_fragment_shader( std::string data );
		void user_inputs( void );
		void update_cam_view( void );
		void update_cam_perspective( void );

	public:
		OpenGL_Manager( GLint nb_textures, std::vector<std::pair<int, size_t *> > vert_tex_pair, bool provided );
		~OpenGL_Manager( void );

		void setup_window( std::string title );
		void setup_array_buffer( Parser *parser );
		void create_shaders( void );
		void setup_communication_shaders( void );
		void load_textures( Parser *parser, t_tex *provided_tex );
		void main_loop( void );
};

#endif
