#include "scop.h"

std::string	trim_spaces(std::string str)
{
	int index = 0;
	std::string new_string;
	while (str[index] == ' ' || str[index] == '\t')
		++index;
	while (str[index])
	{
		if (str[index] == '\t')
			new_string += ' ';
		else
			new_string += str[index];
		if (str[index] == ' ' || str[index] == '\t')
		{
			while (str[index] == ' ' || str[index] == '\t')
				++index;
			--index;
		}
		++index;
	}
	if (new_string.empty())
		return ("");
	if (new_string[new_string.size() - 1] == '\r')
		new_string = new_string.substr(0, new_string.size() - 1);
	if (new_string[new_string.size() - 1] == ' ')
		new_string = new_string.substr(0, new_string.size() - 1);
	return (new_string);
}

void display_special_characters(std::string str)
{
	for (size_t index = 0; str[index]; ++index)
	{
		char c = str[index];

		switch (c)
		{
			case '\\':
				std::cout << "\\";
				break;
			case '\n':
				std::cout << "\\n\n";
				break;
			case '\r':
				std::cout << "\\r";
				break;
			case '\t':
				std::cout << "\\t";
				break;
			default:
				if (isprint(c))
					std::cout << c;
				else
					std::cout << '|' << c + '0' << '|';
				break;
		}
	}
	std::cout << "EOF" << std::endl;
}

/* extract 2 or 3 doubles from line starting at index then store them in t_vertex */
t_vertex parse_vertex( std::string line, size_t index, bool texture )
{
	t_vertex new_vertex;
	size_t tmp_index;

	new_vertex.x = std::stod(line.substr(index), &tmp_index);
	index += tmp_index;
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.y = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	if (texture) {
		// openGL should be doing all that itself, wait and see
		// while (new_vertex.x < 0) {
		// 	++new_vertex.x;
		// }
		// while (new_vertex.y < 0) {
		// 	++new_vertex.y;
		// }
		// while (new_vertex.x > 1) {
		// 	--new_vertex.x;
		// }
		// while (new_vertex.y > 1) {
		// 	--new_vertex.y;
		// }
		// new_vertex.y = 1 - new_vertex.y; //all textures are upside down if I don't do this
		return (new_vertex);
	}
	if (line[index] != ' ') {
		throw InvalidVertexException();
	}
	new_vertex.z = std::stod(line.substr(index + 1), &tmp_index);
	index += tmp_index + 1;
	if (index != line.size()) {
		throw InvalidVertexException();
	}
	return (new_vertex);
}

void set_vertex( t_vertex & vertex, float x, float y, float z )
{
	vertex.x = x;
	vertex.y = y;
	vertex.z = z;
}

// void set_rgb( unsigned int & color, int r, int g, int b )
// {
// 	color = r * 0x10000 + g * 0x100 + b;
// }

// unsigned int get_gradient( t_vertex v )
// {
// 	set_vertex(v, (v.x + EXTREMUM) / (2 * EXTREMUM) * 255,
// 					(v.y + EXTREMUM) / (2 * EXTREMUM) * 255,
// 					(v.z + EXTREMUM) / (2 * EXTREMUM) * 255);
// 	unsigned int res;
// 	set_rgb(res, v.x, v.y, v.z);
// 	return (res);
// }

// double fsubstract( double a, double b )
// {
// 	if (fabs(a - b) < 0.00000001) {
// 		return (0);
// 	}
// 	return (a - b);
// }
