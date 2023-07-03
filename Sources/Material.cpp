#include "scop.h"

Material::Material( std::string name, std::ifstream & indata, std::string & line ) : _name(name), _texture_index(std::string::npos)
{
	// std::cout << "Constructor of Material called" << std::endl;
	if (name.empty()) {
		throw InvalidMltException();
	}

	set_vertex(_color, 1.0f, 0.0f, 0.0f);
	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 3, "Kd ")) {
			set_kd(line);
		} else if (!line.compare(0, 7, "map_Kd ")) {
			_xpm_file = line.substr(7);
		} else if (!line.compare(0, 7, "newmtl ")) {
			break ;
		}
	}
	// std::cout << "color of material " << name << ": " << _color.x << ", " << _color.y << ", " << _color.z << std::endl;
}

Material::~Material( void )
{
	// std::cout << "Destructor of Material called" << std::endl;
}

// ************************************************************************** //
//                                  Private                                   //
// ************************************************************************** //

void Material::set_kd( std::string line )
{
	_color = parse_vertex(line, 3, false);

	if (_color.x < 0 || _color.x > 1 || _color.y < 0 || _color.y > 1
		|| _color.z < 0 || _color.z > 1) {
		throw InvalidRGBException();
	}

	// std::cout << "rgb of material set to: " << _color << std::endl;
}

// ************************************************************************** //
//                                  Public                                    //
// ************************************************************************** //

t_vertex Material::get_color( void )
{
	return (_color);
}

std::string Material::get_name( void )
{
	return (_name);
}

std::string Material::get_xpm( void )
{
	return (_xpm_file);
}

size_t *Material::get_texture_index( void )
{
	return (&_texture_index);
}

void Material::set_texture_index( size_t index )
{
	_texture_index = (int)index;
}