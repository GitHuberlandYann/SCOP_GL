#include "scop.h"

Parser::Parser( std::string root ) : _root(root), _face_mode(UNSET), _current_used_material(NULL), _number_vertices(0)
{
	std::cout << "Constructor of Parser called" << std::endl << std::endl;
	set_vertex(_max_box, -10000, -10000, -10000);
	set_vertex(_min_box, 10000, 10000, 10000);
	set_vertex(_null_vertex, 0.0f, 0.0f, 0.0f);
}

Parser::~Parser( void )
{
	std::cout << "Destructor of Parser called" << std::endl;
	
	std::vector<t_vertex *>::iterator v_it = _vertices.begin();
	std::vector<t_vertex *>::iterator v_ite = _vertices.end();
	for (; v_it != v_ite; v_it++) {
		delete *v_it;
	}
	_vertices.clear();

	std::vector<t_vertex *>::iterator vt_it = _vertices_textures.begin();
	std::vector<t_vertex *>::iterator vt_ite = _vertices_textures.end();
	for (; vt_it != vt_ite; vt_it++) {
		delete *vt_it;
	}
	_vertices_textures.clear();

	std::vector<t_vertex *>::iterator vn_it = _vertices_normals.begin();
	std::vector<t_vertex *>::iterator vn_ite = _vertices_normals.end();
	for (; vn_it != vn_ite; vn_it++) {
		delete *vn_it;
	}
	_vertices_normals.clear();

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();
	
	for (; it != ite; it++) {
		delete *it;
	}
	_faces.clear();

	std::vector<Material *>::iterator m_it = _materials.begin();
	std::vector<Material *>::iterator m_ite = _materials.end();
	
	for (; m_it != m_ite; m_it++) {
		delete *m_it;
	}
	_materials.clear();

	std::vector<t_tex *>::iterator t_it = _textures.begin();
	std::vector<t_tex *>::iterator t_ite = _textures.end();
	
	for (; t_it != t_ite; t_it++) {
		SOIL_free_image_data((*t_it)->texture);
		delete *t_it;
	}
	_textures.clear();

	_nbvert_index_textures.clear();
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
	t_vertex *res = new t_vertex;
	set_vertex(*res, vertex.x, vertex.y, vertex.z);
	_vertices.push_back(res);
}

void Parser::push_vertextn( t_vertex vertex, bool norm )
{
	t_vertex *res = new t_vertex;
	set_vertex(*res, vertex.x, vertex.y, vertex.z);
	if (norm) {
		_vertices_normals.push_back(res);
	} else {
		_vertices_textures.push_back(res);
	}
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

	v_num = check_vindex(v_num);
	switch (_face_mode) {
		case UNSET:
			throw InvalidFaceException();
		case ONLY_V:
			face->add_vertex(_vertices[v_num], &_null_vertex, &_null_vertex);
			break;
		case ONLY_VT:
			vt_num = check_vtindex(vt_num);
			face->add_vertex(_vertices[v_num], _vertices_textures[vt_num], &_null_vertex);
			break;
		case ONLY_VN:
			vn_num = check_vnindex(vn_num);
			face->add_vertex(_vertices[v_num], &_null_vertex, _vertices_normals[vn_num]);
			break;
		case VTN:
			vt_num = check_vtindex(vt_num);
			vn_num = check_vnindex(vn_num);
			face->add_vertex(_vertices[v_num], _vertices_textures[vt_num], _vertices_normals[vn_num]);
	}
}

/* check if face line is correct, return instance of class Face */
void Parser::push_face( std::string line )
{
	if (!_materials.empty() && !_current_used_material) {
		throw MltlibNoUseException();
	}

	Face *new_face = new Face(_current_used_material);
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
	if (_current_used_material) {
		_nbvert_index_textures[_nbvert_index_textures.size() - 1].first += (face_size - 2) * 3;
	}
}

/* read .mtl file and store its info if correct */
void Parser::add_materials( std::string file )
{
	if (!_materials.empty()) {
		throw DoubleMltlibException();
	} else if (file.empty()) {
		throw NoMltlibFileException();
	} else if (file.size() < 4 || file.compare(file.size() - 4, 4, ".mtl")) {
		throw MltExtensionException();
	} else if (!_vertices.empty()) {
		throw MltlibNoStartException();
	}

	file = _root + file;
	std::cout << "Opening file " << file << std::endl;
	std::ifstream indata(file.c_str());
	if (!indata.is_open()) {
		throw InvalidMltFileException();
	}
	std::string line;
	while (!indata.eof()) {
		if (line.compare(0, 7, "newmtl ")) {
			std::getline(indata, line);
			line = trim_spaces(line);
		}
		// display_special_characters(line);
		if (line.empty() || line[0] == '#') {
			continue ;
		} else if (!line.compare(0, 7, "newmtl ")) {
			_materials.push_back(new Material(line.substr(7), indata, line));
		}
	}
	indata.close();
}

void Parser::set_material( std::string name )
{
	std::vector<Material *>::iterator it = _materials.begin();
	std::vector<Material *>::iterator ite = _materials.end();

	for (; it != ite; it++) {
		if (!name.compare(0, name.size(), (*it)->get_name())) {
			_current_used_material = *it;
			_nbvert_index_textures.push_back(std::pair<int, size_t *>(0, (*it)->get_texture_index()));
			return ;
		}
	}
	std::cout << "Material: " << name << std::endl;
	throw NoMatchingMaterialException();
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

int Parser::check_duplicate_xpm( std::vector<Material *>::iterator it, std::vector<Material *>::iterator ite )
{
	std::string current_xpm = (*ite)->get_xpm();
	for (; it != ite; it++) {
		if (!current_xpm.compare((*it)->get_xpm())) {
			(*ite)->set_texture_index(*(*it)->get_texture_index());
			return (0);
		}
	}
	return (1);
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
			push_vertextn(parse_vertex(line, 3, true), 0);
		} else if (!line.compare(0, 3, "vn ")) {
			push_vertextn(parse_vertex(line, 3, false), 1);
		} else if (!line.compare(0, 2, "f ")) {
			push_face(line); //, distribution(generator)); //generate random int from [50:200]
		} else if (!line.compare(0, 7, "usemtl ")) {
		 	set_material(line.substr(7));
		} else if (!line.compare(0, 7, "mtllib ")) {
			add_materials(line.substr(7));
		}
	}
	indata.close();

	if (_faces.empty()) {
		throw EmptyObjectException();
	}
}

/* loop through materials generated by .mlt file and load their textures */
void Parser::load_textures( void )
{
	std::vector<Material *>::iterator it = _materials.begin();
	std::vector<Material *>::iterator ite = _materials.end();

	size_t cnt = 0, dup_cnt = 0;
	std::string tex_file;
	char *tex_file_str;
	for (; it != ite; it++) {
		if ((*it)->get_xpm().empty()) {
			continue ;
		} else if (!check_duplicate_xpm(_materials.begin(), it)) {
			++dup_cnt;
			continue ;
		}
		t_tex *new_tex = new t_tex;
		_textures.push_back(new_tex);
		tex_file = _root + (*it)->get_xpm();
		tex_file_str = &tex_file[0];
		new_tex->texture = SOIL_load_image(tex_file_str, &new_tex->width, &new_tex->height, 0, SOIL_LOAD_RGB);
		if (!new_tex->texture) {
			std::cerr << "failed to load image " << tex_file << " because:" << std::endl << SOIL_last_result() << std::endl;
			throw SOILFailureException();
		} else {
			std::cout << tex_file << " loaded successfully" << std::endl;
		}
		(*it)->set_texture_index(cnt);
		++cnt;
	}
	(!dup_cnt)
		? std::cout << " ---- number of textures loaded: " << cnt << " ----" << std::endl << std::endl
		: std::cout << " ---- number of textures loaded: " << cnt + dup_cnt << " -> " << cnt << " ----" << std::endl << std::endl;
}

/* use minmax_box to center object on {0, 0, 0}
/  also normalize it to fit in [-0.5;0.5]        */
void Parser::center_object( void )
{
	t_vertex central_axis = {(_max_box.x + _min_box.x) / 2,
								(_max_box.y + _min_box.y) / 2,
								(_max_box.z + _min_box.z) / 2};
	
	_max_box.x -= central_axis.x;
	_max_box.y -= central_axis.y;
	_max_box.z -= central_axis.z;
	_min_box.x -= central_axis.x;
	_min_box.y -= central_axis.y;
	_min_box.z -= central_axis.z;

	float normalizer = 0.5f / get_extremum();

	std::vector<t_vertex *>::iterator it = _vertices.begin();
	std::vector<t_vertex *>::iterator ite = _vertices.end();

	for (; it != ite; it++) {
		set_vertex(**it, ((*it)->x - central_axis.x) * normalizer,
						((*it)->y - central_axis.y) * normalizer,
						((*it)->z - central_axis.z) * normalizer);
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
	std::cout << "\t-Number of materials: " << _materials.size() << std::endl;
	// for (size_t index = 0; index < _nbvert_index_textures.size(); index++) {
	// 	if (!index)
	// 		std::cout << "\t-material pairs [nb_vert; tex_index]:" << std::endl;
	// 	std::cout << "\t     " << _nbvert_index_textures[index].first << ", " << *_nbvert_index_textures[index].second << std::endl;
	// }
	std::cout << "\t-Number of textures: " << _textures.size() << std::endl;
	std::cout << "\t-box x[" << _min_box.x << ':' << _max_box.x << ']' << std::endl;
	std::cout << "\t     y[" << _min_box.y << ':' << _max_box.y << ']' << std::endl;
	std::cout << "\t     z[" << _min_box.z << ':' << _max_box.z << ']' << std::endl;
	std::cout << std::endl << " ----------------------" << std::endl << std::endl;
}

unsigned int Parser::get_number_vertices( void )
{
	return (_number_vertices);
}

int Parser::get_number_textures( void )
{
	return (_textures.size());
}

std::vector<t_tex *> Parser::get_textures( void )
{
	return (_textures);
}

std::vector<std::pair<int, size_t *> > Parser::get_nbvert_index_textures( void )
{
	return (_nbvert_index_textures);
}

void Parser::fill_vertex_array(GLfloat *vertices)
{
	size_t index = 0;

	std::vector<Face *>::iterator it = _faces.begin();
	std::vector<Face *>::iterator ite = _faces.end();

	for (; it != ite; it++) {
		(*it)->fill_vertex_array(vertices, index);
	}

	// std::cout << "vertices filled up to index " << index << std::endl;
}
