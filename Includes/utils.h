#ifndef UTILS_HPP
# define UTILS_HPP

# include "scop.h"

std::string	trim_spaces( std::string str );
std::string get_file_content( std::string file_name );
void display_special_characters( std::string str );
t_vertex parse_vertex( std::string line, size_t index, bool texture );
void set_vertex( t_vertex & vertex, float x, float y, float z );

#endif
