#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glbinding/gl/gl.h>
#include <glbinding/Binding.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

using namespace gl;

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera{
public:
	Camera(glm::vec3 position, glm::vec3 up, float _fov);
	
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xoffset,
		float yoffset, GLboolean constrainPitch = true);	
	void processMouseScroll(float yoffset);
	glm::mat4 getCameraCombinedMatrix();
	void setPerspectiveMatrix(int width, int height);
	

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	float fov;
	// Euler Angles
	float yaw;
	float pitch;
	// Camera options
	float movementSpeed=3.0;
	float mouseSensitivity=0.5;
	float zoom=1.0;	
protected:
	void updateCameraVectors();
	glm::mat4 mxPerspective;
	float YAW         = -90.0f;
	float PITCH       =  0.0f;
	float SPEED       =  2.5f;
	float SENSITIVITY =  0.1f;
	float ZOOM        =  45.0f;
};

#endif //CAMERA_HPP
