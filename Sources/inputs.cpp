#include "scop.h"

void OpenGL_Manager::user_inputs( void )
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GL_TRUE);
	
	GLint key_rotation_z = (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS);
	GLint key_rotation_x = (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS);
	GLint key_rotation_y = (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS);
	if (key_rotation_z || key_rotation_y || key_rotation_x) {
		_rotation.x += key_rotation_x * _rotation_speed;
		_rotation.y += key_rotation_y * _rotation_speed;
		_rotation.z += key_rotation_z * _rotation_speed;
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(_rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(_uniModel, 1, GL_FALSE, glm::value_ptr(model));
	}

	GLint key_rotation_speed = (glfwGetKey(_window, GLFW_KEY_KP_ADD) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS);
	if (key_rotation_speed && _rotation_speed + 0.1f * key_rotation_speed >= 0)
		_rotation_speed += 0.1f * key_rotation_speed;

	if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_PRESS && ++_key_color_mode == 1) {
		++_color_mode;
		if (_color_mode == MATERIAL && !_vtp_size)
			++_color_mode;
		if (_color_mode == TEXTURE && (!_nb_textures || _nb_textures == _omore_tex))
			++_color_mode;
		if (_color_mode == PROVIDED && !_omore_tex)
			++_color_mode;
		if (_color_mode == LAST)
			_color_mode = DEFAULT;
		glUniform1i(_uniColorMode, _color_mode);
	} else if (glfwGetKey(_window, GLFW_KEY_C) == GLFW_RELEASE)
		_key_color_mode = 0;

	if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_PRESS && ++_key_depth == 1) {
		if (glIsEnabled(GL_DEPTH_TEST))
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
	} else if (glfwGetKey(_window, GLFW_KEY_B) == GLFW_RELEASE)
		_key_depth = 0;

	if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_PRESS && ++_key_fill == 1) {
		++_fill;
		if (_fill == F_LAST)
			_fill = FILL;
		switch (_fill) {
			case FILL:
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case LINE:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;
			case POINT:
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;
		}
	} else if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_RELEASE)
		_key_fill = 0;
	
	GLint key_point_size = (glfwGetKey(_window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS);
	if (key_point_size && _point_size + 0.1f * key_point_size >= 1.0f && _point_size + 0.1f * key_point_size < 10.0f) {
		_point_size += 0.1f * key_point_size;
		glPointSize(_point_size);
	}

	GLint key_zoom = (glfwGetKey(_window, GLFW_KEY_EQUAL) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_MINUS) == GLFW_PRESS);
	if (key_zoom && _zoom + 0.1f * key_zoom >= 0) {
		_zoom += 0.1f * key_zoom;
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(_zoom));
		glUniformMatrix4fv(_uniScale, 1, GL_FALSE, glm::value_ptr(scale));
	}

	GLint key_light = (glfwGetKey(_window, GLFW_KEY_Z) == GLFW_PRESS);

	GLint key_col_r = (glfwGetKey(_window, GLFW_KEY_KP_7) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_4) == GLFW_PRESS);
	GLint key_col_g = (glfwGetKey(_window, GLFW_KEY_KP_8) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_5) == GLFW_PRESS);
	GLint key_col_b = (glfwGetKey(_window, GLFW_KEY_KP_9) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_6) == GLFW_PRESS);
	if (key_col_r || key_col_g || key_col_b) {
		if (!key_light) {
			_background_color.x = glm::clamp(_background_color.x + key_col_r * 0.01f, 0.f, 1.f);
			_background_color.y = glm::clamp(_background_color.y + key_col_g * 0.01f, 0.f, 1.f);
			_background_color.z = glm::clamp(_background_color.z + key_col_b * 0.01f, 0.f, 1.f);
			glClearColor(_background_color.x, _background_color.y, _background_color.z, 1.0f);
		} else {
			_light_col.x = glm::clamp(_light_col.x + key_col_r * 0.01f, 0.f, 1.f);
			_light_col.y = glm::clamp(_light_col.y + key_col_g * 0.01f, 0.f, 1.f);
			_light_col.z = glm::clamp(_light_col.z + key_col_b * 0.01f, 0.f, 1.f);
			glUniform3fv(_uniLightColor, 1, glm::value_ptr(_light_col));
		}
	}

	GLint key_next_section = (glfwGetKey(_window, GLFW_KEY_N) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_M) == GLFW_PRESS);
	if (key_next_section && ++_key_section == 1) {
		_section += key_next_section;
		if (_section == -1)
			_section = _vtp_size;
		else if (_section == (int)_vtp_size + 1)
			_section = 0;
	} else if (!key_next_section)
		_key_section = 0;
	
	if (glfwGetKey(_window, GLFW_KEY_I) == GLFW_PRESS && ++_key_invert == 1) {
		++_invert_col;
		if (_invert_col == 3)
			_invert_col = 0;
		glUniform1i(_uniInvert, _invert_col);
	} else if (glfwGetKey(_window, GLFW_KEY_I) == GLFW_RELEASE)
		_key_invert = 0;
	
	if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_PRESS && ++_key_use_light == 1) {
		if (_can_light) {
			_use_light = !_use_light;
			glUniform1i(_uniUseLight, _use_light);
		}
	} else if (glfwGetKey(_window, GLFW_KEY_L) == GLFW_RELEASE)
		_key_use_light = 0;
	
	/* camera and light work */
	GLint key_cam_v = (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS);
	if (key_cam_v) {
		if (!key_light) {
			_cam_angles.y += key_cam_v;
			if (_cam_angles.y < -180.0f)
				_cam_angles.y = 179.0f;
			else if (_cam_angles.y > 180.0f)
				_cam_angles.y = -179.0f;
			// std::cout << "current angle y cam: " << _cam_angles.y << std::endl;
		} else {
			_light_angles.y = glm::clamp(_light_angles.y + key_cam_v * (_rotation_speed - 0.5f), -90.0f, 90.0f);
		}
	}
	GLint key_cam_h = (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS);
	if (key_cam_h) {
		if (!key_light) {
			_cam_angles.x += key_cam_h * (_rotation_speed - 0.5f);
			if (_cam_angles.x < 0.0f)
				_cam_angles.x = 359.0f;
			else if (_cam_angles.x > 360.0f)
				_cam_angles.x = 1.0f;
		} else {
			_light_angles.x += key_cam_h * (_rotation_speed - 0.5f);
			if (_light_angles.x < 0.0f)
				_light_angles.x = 359.0f;
			else if (_light_angles.x > 360.0f)
				_light_angles.x = 1.0f;
		}
	}
	GLint key_cam_z = (glfwGetKey(_window, GLFW_KEY_KP_1) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_0) == GLFW_PRESS);
	if (key_cam_z) {
		_cam_pos.z += key_cam_z * 0.05f * (_rotation_speed - 0.5f);
	}
	GLint key_cam_move = (glfwGetKey(_window, GLFW_KEY_KP_3) == GLFW_PRESS) - (glfwGetKey(_window, GLFW_KEY_KP_2) == GLFW_PRESS);
	if (key_cam_move) {
		glm::vec3 cam_director = glm::vec3(glm::cos(glm::radians(_cam_angles.x)),
											glm::sin(glm::radians(_cam_angles.x)),
											glm::sin(glm::radians(_cam_angles.y)));
		if (key_cam_move == 1)
			_cam_pos += 0.01f * (_rotation_speed - 0.5f) * cam_director;
		else
			_cam_pos -= 0.01f * (_rotation_speed - 0.5f) * cam_director;
	}

	if ((!key_light && (key_cam_v || key_cam_h)) || key_cam_z || key_cam_move)
	{
		glm::vec3 cam_director = glm::vec3(glm::cos(glm::radians(_cam_angles.x)),
											glm::sin(glm::radians(_cam_angles.x)),
											glm::sin(glm::radians(_cam_angles.y)));
		glm::mat4 view = glm::lookAt(_cam_pos, _cam_pos + cam_director, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(_uniView, 1, GL_FALSE, glm::value_ptr(view));
	}
	if (key_light && (key_cam_v || key_cam_h)) {
		_light_pos = 2.0f * glm::vec3(glm::cos(glm::radians(_light_angles.x)),
								glm::sin(glm::radians(_light_angles.x)),
								glm::sin(glm::radians(_light_angles.y)));
		glUniform3fv(_uniLightPos, 1, glm::value_ptr(_light_pos));
	}
}

// void cursor_position_callback( GLFWwindow* window, double xpos, double ypos )
// {
// 	(void)window;
// 	std::cout << "current mouse position: [" << xpos << ", " << ypos << ']' << std::endl;
// }
