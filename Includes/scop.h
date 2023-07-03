#ifndef SCOP_H
# define SCOP_H

# include <iostream>
# include <string>
# include <thread>
# include <fstream>
# include <vector>
# include <math.h>

typedef struct {
	float x;
	float y;
	float z;
}			t_vertex;

# define GLEW_STATIC
# include <GL/glew.h> // must be before glfw
# include "GLFW/glfw3.h"
# include "SOIL/SOIL.h"
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtc/type_ptr.hpp>

# include "Exception.hpp"
# include "utils.h"
# include "Parser.hpp"
# include "Face.hpp"
# include "OpenGL_Manager.hpp"

#endif
