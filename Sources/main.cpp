#include "scop.h"

static std::string get_file( int ac, char **av )
{
	if (ac == 1) {
		return ("Resources/planet.obj");
	} else if (ac <= 3) {
		std::string res(av[1]);

		if (res.size() >= 4 && (!res.compare(res.size() - 4, 4, ".obj") || !res.compare(res.size() - 4, 4, ".OBJ"))) {
			return (res);
		}
	}
	std::cerr << "Usage: ./scop <path-to-file>.obj [<path-to-additional-texture>]" << std::endl;
	return ("");
}

static t_tex *get_texture( int ac, char **av) {
	if (ac != 3) {
		return (NULL);
	}
	t_tex *res = new t_tex;

	res->texture = SOIL_load_image(av[2], &res->width, &res->height, 0, SOIL_LOAD_RGB);
	if (!res->texture) {
		std::cerr << "failed to load image " << av[2] << " because:" << std::endl << SOIL_last_result() << std::endl;
		exit(1);
	}
	std::cout << av[2] << " loaded successfully" << std::endl;
	return (res);
}

static std::string get_root( std::string file )
{
	size_t index = file.rfind('/');
	if (index == std::string::npos)
		return ("");
	return (file.substr(0, index + 1));
}

int main( int ac, char **av )
{
	std::string file = get_file(ac, av);
	if (file.empty())
		return (1);

	std::cout << " ---- Hello ----" << std::endl << std::endl;
	t_tex *provided_tex = get_texture(ac, av);
	Parser *parser = new Parser(get_root(file));

	try {
		parser->parse(file);
		std::cout << std::endl;
		parser->load_textures();
		parser->center_object();
		parser->display_content();
	} catch (std::exception & e) {
		std::cerr << e.what() << std::endl;
		delete parser;
		if (provided_tex)
			SOIL_free_image_data(provided_tex->texture);
		delete provided_tex;
		std::cout << std::endl << " ---- Goodbye ----" << std::endl;
		return (1);
	}

	OpenGL_Manager *render = new OpenGL_Manager(parser->get_number_textures(), parser->get_nbvert_index_textures(), !!provided_tex);

	render->setup_window(file);
	render->setup_array_buffer(parser);
	std::cout << std::endl;
	render->create_shaders();
	render->setup_communication_shaders();
	render->load_textures(parser, provided_tex);
	delete parser;
	std::cout << std::endl;
	render->main_loop();

	delete render;
	if (provided_tex)
		SOIL_free_image_data(provided_tex->texture);
	delete provided_tex;

	std::cout << std::endl << " ---- Goodbye ----" << std::endl;
	return (0);
}
