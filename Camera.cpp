#include "Camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float _fov){
	//setPosition(viewPoint);
	position = pos;
	worldUp = up;
	yaw  = YAW;
	pitch = PITCH;
	fov = _fov;
	updateCameraVectors();
}

glm::mat4 Camera::getCameraCombinedMatrix(){
	return mxPerspective*glm::lookAt(position, position + front, up);
	//return mxPerspective;
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime){
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float xoffset, 
	float yoffset, GLboolean constrainPitch){
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw		+= xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch){
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

#include <iostream>

void Camera::processMouseScroll(float yoffset){
	yoffset/=100.0;
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 0.5f)
		zoom = 0.5f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors(){
	// Calculate the new Front vector
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, worldUp));  
	// Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up    = glm::normalize(glm::cross(right, front));
}

#include <iostream>

void Camera::setPerspectiveMatrix(
		int width, int height){
	mxPerspective = glm::perspective(
		glm::radians(fov*zoom/2.f), (float)width/(float)height,
		0.1f, 100.0f);
	//mxPerspective = glm::mat4();

	//glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)
}

