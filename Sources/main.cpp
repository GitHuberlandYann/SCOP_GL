#include "scop.h"

static std::string get_file( int ac, char **av )
{
	if (ac == 1) {
		return ("Resources/planet.obj");
	} else if (ac == 2) {
		std::string res(av[1]);

		if (res.size() >= 4 && (!res.compare(res.size() - 4, 4, ".obj") || !res.compare(res.size() - 4, 4, ".OBJ"))) {
			return (res);
		}
	}
	std::cerr << "Usage: ./scop <path-to-file>.obj" << std::endl;
	return ("");
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
		std::cout << std::endl << " ---- Goodbye ----" << std::endl;
		return (1);
	}

	OpenGL_Manager *render = new OpenGL_Manager(parser->get_number_textures(), parser->get_nbvert_index_textures());

	render->setup_window();
	render->setup_array_buffer(parser);
	std::cout << std::endl;
	render->create_shaders();
	render->setup_communication_shaders();
	render->load_textures(parser);
	delete parser;
	std::cout << std::endl;
	render->main_loop();

	delete render;

	std::cout << std::endl << " ---- Goodbye ----" << std::endl;
	return (0);
}
