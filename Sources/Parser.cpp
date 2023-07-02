#include "scop.h"

Parser::Parser( std::string root ) : _root(root), _face_mode(UNSET), _number_vertices(0)
{
	std::cout << "Constructor of Parser called" << std::endl;
	set_vertex(_max_box, -10000, -10000, -10000);
	set_vertex(_min_box, 10000, 10000, 10000);
}

Parser::~Parser( void )
{
	std::cout << "Destructor of Parser called" << std::endl;
	_vertices.clear();
	_vertices_textures.clear();
	_vertices_normals.clear();

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();
	
	for (; it != ite; it++) {
		delete *it;
	}
	_faces.clear();
}


// ************************************************************************** //
//                                Private                                     //
// ************************************************************************** //

/* set min and max box values if needed and push vertex to vector */
void Parser::push_vertex( t_vertex vertex )
{
	if (vertex.x > _max_box.x)
		_max_box.x = vertex.x;
	if (vertex.y > _max_box.y)
		_max_box.y = vertex.y;
	if (vertex.z > _max_box.z)
		_max_box.z = vertex.z;
	if (vertex.x < _min_box.x)
		_min_box.x = vertex.x;
	if (vertex.y < _min_box.y)
		_min_box.y = vertex.y;
	if (vertex.z < _min_box.z)
		_min_box.z = vertex.z;
	_vertices.push_back(vertex);
}

int Parser::check_vindex( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices.size()) {
		throw InvalidFaceException();
	}
	return (num - 1);
}

int Parser::check_vtindex( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices_textures.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices_textures.size()) {
		throw InvalidFaceException();
	}
	return (num - 1);
}

int Parser::check_vnindex( int num )
{
	if (num == 0) {
		throw InvalidFaceException();
	} else if (num < 0) {
		num += _vertices_normals.size() + 1;
		if (num < 0) {
			throw InvalidFaceException();
		}
	} else if (num > (int) _vertices_normals.size()) {
		throw InvalidFaceException();
	}
	return (num - 1);
}


/* parse v[/vt/vn] from f line and get given vertices from storage */
void Parser::add_vertex_face( Face *face, std::string line, size_t & index )
{
	size_t tmp_index;

	int v_num = std::stoi(line.substr(index), &tmp_index);
	int vt_num = 0;
	int vn_num = 0;
	index += tmp_index;
	if (line[index] == ' ' || !line[index]) {
		if (_face_mode == UNSET) {
			_face_mode = ONLY_V;
		} else if (_face_mode != ONLY_V) {
			throw InvalidFaceException();
		}
	} else if (line[index] != '/') {
		throw InvalidFaceException();
	} else {
		if (line[index + 1] == '/') {
			if (_face_mode == UNSET) {
				_face_mode = ONLY_VN;
			} else if (_face_mode != ONLY_VN) {
				throw InvalidFaceException();
			}
			vn_num = std::stoi(line.substr(index + 2), &tmp_index);
			index += tmp_index + 2;
			if (line[index] && line[index] != ' ') {
				throw InvalidFaceException();
			}
		} else if (std::isdigit(line[index + 1]) || line[index + 1] == '-') {
			vt_num = std::stoi(line.substr(index + 1), &tmp_index);
			index += tmp_index + 1;
			if (line[index] == ' ' || !line[index]) {
				if (_face_mode == UNSET) {
					_face_mode = ONLY_VT;
				} else if (_face_mode != ONLY_VT) {
					throw InvalidFaceException();
				}
			} else if (line[index] != '/') {
				throw InvalidFaceException();
			} else {
				if (std::isdigit(line[index + 1]) || line[index + 1] == '-') {
					vn_num = std::stoi(line.substr(index + 1), &tmp_index);
					index += tmp_index + 1;
					if (line[index] == ' ' || !line[index]) {
						if (_face_mode == UNSET) {
							_face_mode = VTN;
						} else if (_face_mode != VTN) {
							throw InvalidFaceException();
						}
					} else {
						throw InvalidFaceException();
					}
				} else {
					throw InvalidFaceException();
				}
			}
		} else {
			throw InvalidFaceException();
		}
	}

	switch (_face_mode) {
		case UNSET:
			throw InvalidFaceException();
		case ONLY_V:
			face->add_vertex(check_vindex(v_num), -1, -1);
			break;
		case ONLY_VT:
			face->add_vertex(check_vindex(v_num), check_vtindex(vt_num), -1);
			break;
		case ONLY_VN:
			face->add_vertex(check_vindex(v_num), -1, check_vnindex(vn_num));
			break;
		case VTN:
			face->add_vertex(check_vindex(v_num), check_vtindex(vt_num), check_vnindex(vn_num));
	}
}

/* check if face line is correct, return instance of class Face */
void Parser::push_face( std::string line )
{
	Face *new_face = new Face();
	_faces.push_back(new_face);
	size_t index = 2;

	while (std::isdigit(line[index]) || line[index] == '-') {
		add_vertex_face( new_face, line, index );
		if (line[index] == ' ') {
			++index;
		}
	}
	size_t face_size = new_face->get_size();
	if (face_size < 3) {
		throw InvalidFaceException();
	}

	_number_vertices += (face_size - 2) * 3; // like this for now, but later on we will reuse some vertices to gain memory space
}

float Parser::get_extremum( void )
{
	float res = _max_box.x;
	if (_max_box.y > res) {
		res = _max_box.y;
	}
	if (_max_box.z > res) {
		res = _max_box.z;
	}
	if (-_min_box.x > res) {
		res = -_min_box.x;
	}
	if (-_min_box.y > res) {
		res = -_min_box.y;
	}
	if (-_min_box.z > res) {
		res = -_min_box.z;
	}
	return (res);
}

// ************************************************************************** //
//                                Public                                      //
// ************************************************************************** //

/* read .obj file provided and store its informations if they are correct */
void Parser::parse( std::string file )
{
	// std::default_random_engine generator;
	// std::uniform_int_distribution<int> distribution(50, 200);

	std::cout << "Opening file " << file << std::endl;
	std::ifstream indata(file.c_str());
	if (!indata.is_open()) {
		throw InvalidFileException();
	}
	std::string line;
	while (!indata.eof()) {
		std::getline(indata, line);
		line = trim_spaces(line);
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 2, "v ")) {
			push_vertex(parse_vertex(line, 2, false));
		} else if (!line.compare(0, 3, "vt ")) {
			_vertices_textures.push_back(parse_vertex(line, 3, true));
		} else if (!line.compare(0, 3, "vn ")) {
			_vertices_normals.push_back(parse_vertex(line, 3, false));
		} else if (!line.compare(0, 2, "f ")) {
			push_face(line); //, distribution(generator)); //generate random int from [50:200]
		} //else if (!line.compare(0, 7, "usemtl ")) {
		// 	set_material(line.substr(7));
		// } else if (!line.compare(0, 7, "mtllib ")) {
		// 	add_materials(line.substr(7));
		// }
	}
	indata.close();

	if (_faces.empty()) {
		throw EmptyObjectException();
	}
}

/* use minmax_box to center object on {0, 0, 0}
/  also normalize it to fit in [-0.5;0.5]        */
void Parser::center_object( void )
{
	t_vertex central_axis = {(_max_box.x + _min_box.x) / 2,
								(_max_box.y + _min_box.y) / 2,
								(_max_box.z + _min_box.z) / 2};

	// std::cout << "max: {" << _max_box.x << ", " << _max_box.y << ", " << _max_box.z << "}" << std::endl;
	// std::cout << "min: {" << _min_box.x << ", " << _min_box.y << ", " << _min_box.z << "}" << std::endl;
	// std::cout << "axis: {" << central_axis.x << ", " << central_axis.y << ", " << central_axis.z << "}" << std::endl;
	
	_max_box.x -= central_axis.x;
	_max_box.y -= central_axis.y;
	_max_box.z -= central_axis.z;
	_min_box.x -= central_axis.x;
	_min_box.y -= central_axis.y;
	_min_box.z -= central_axis.z;

	float normalizer = 0.5f / get_extremum();

	std::vector<t_vertex>::iterator it = _vertices.begin();
	std::vector<t_vertex>::iterator ite = _vertices.end();

	for (; it != ite; it++) {
		set_vertex(*it, (it->x - central_axis.x) * normalizer,
						(it->y - central_axis.y) * normalizer,
						(it->z - central_axis.z) * normalizer);
	}
}

void Parser::display_content( void )
{
	const std::string modes[] = {"UNSET", "ONLY_V", "ONLY_VT", "ONLY_VN", "VTN"};

	std::cout << std::endl << " ---- content ----" << std::endl << std::endl;
	std::cout << "\t-Face Mode: " << modes[_face_mode] << std::endl;
	std::cout << "\t-Number of vertices: " << _vertices.size() << std::endl;
	std::cout << "\t-Number of vertices_textures: " << _vertices_textures.size() << std::endl;
	std::cout << "\t-Number of vertices_normals: " << _vertices_normals.size() << std::endl;
	std::cout << "\t-Number of faces: " << _faces.size() << std::endl;
	std::cout << "\t-Number of vertices passed to shader: " << _number_vertices << std::endl;
	// std::cout << "\t-Number of materials: " << _materials.size() << std::endl;
	std::cout << "\t-box x[" << _min_box.x << ':' << _max_box.x << ']' << std::endl;
	std::cout << "\t     y[" << _min_box.y << ':' << _max_box.y << ']' << std::endl;
	std::cout << "\t     z[" << _min_box.z << ':' << _max_box.z << ']' << std::endl;
	std::cout << std::endl << " ----------------------" << std::endl << std::endl;
}

unsigned int Parser::get_number_vertices( void )
{
	return (_number_vertices);
}

void Parser::fill_vertex_array(GLfloat *vertices)
{
	size_t index = 0;

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->fill_vertex_array(vertices, index, _face_mode, _vertices, _vertices_textures, _vertices_normals);
	}

	std::cout << "vertices filled up to index " << index << std::endl;
}
