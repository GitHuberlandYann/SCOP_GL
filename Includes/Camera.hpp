#ifndef CAMERA_H
# define CAMERA_H

# include "scop.h"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	UP,
	DOWN
};
	// _cam_angles = glm::vec2(225.0f, -37.0f);
# define YAW          -90.0f
# define PITCH        0.0f
# define SPEED        0.01f
# define SENSITIVITY  0.001f
# define ZOOM         45.0f

void cursor_position_callback( GLFWwindow* window, double xpos, double ypos );
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset );

class Camera
{
	private:
		glm::vec3 _position;
		glm::vec3 _front;
		glm::vec3 _up;
		glm::vec3 _right;
		glm::vec3 _world_up;
		float _yaw;
		float _pitch;
		float _movement_speed;
		float _mouse_sensitivity;
		float _zoom;
    	void updateCameraVectors( void );
	
	public:
		bool _mouse_update, _scroll_update;

		Camera( glm::vec3 position );
		~Camera( void );

		glm::mat4 getViewMatrix( void );
		glm::mat4 getPerspectiveMatrix( void );
		void processKeyboard( Camera_Movement direction );
		void processMouseMovement( float x_offset, float y_offset );
		void processMouseScroll( float offset );
};

#endif
