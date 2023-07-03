#ifndef MATERIAL_HPP
# define MATERIAL_HPP

class Material {
	private:
		std::string _name;
		t_vertex _color; //kd
		std::string _xpm_file;
		size_t _texture_index; //map_kd

		void set_kd( std::string line );

	public:
		Material( std::string name, std::ifstream & indata, std::string & line );
		~Material( void );

		t_vertex get_color( void );
		std::string get_name( void );
		std::string get_xpm( void );
		size_t *get_texture_index( void );
		void set_texture_index( size_t index );

};

#endif
