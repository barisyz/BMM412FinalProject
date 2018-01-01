#include "Camera.h"

using namespace std;

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
	c_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.001f, 2000.0f);
	
	//example start state
	c_position = { 0, 0, -1 };
	c_direction = { 0, 0, 1 };
	c_upVector = { 0, 1, 0 };
	c_rightVector = { -1, 0, 0 };

	verticalAngle = 0;
	horizontalAngle = 0;
}

glm::vec3 Camera::GetPosition()
{
	return c_position;
}

void Camera::MoveTo(glm::vec3 location) {

	c_position = location;

}
void Camera::MoveX(float direction) {

	c_position.x += direction / 5.0f;
}
void Camera::MoveY(float direction) {

	c_position.y += direction / 5.0f;
}
void Camera::MoveZ(float direction) {

	c_position.z += direction / 5.0f;
}
void Camera::MoveForward(float direction) {

	c_position += c_direction * direction / 5.0f;
}
void Camera::MoveHorizontal(float direction) {

	c_position += c_rightVector * direction / 5.0f;
}
void Camera::MoveUpward(float direction) {

	c_position += c_upVector * direction / 5.0f;
}

void Camera::Rotate(float ypos, float xpos) {

	horizontalAngle += sensitivity * xpos;
	verticalAngle += sensitivity * ypos;

	c_direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	c_rightVector = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	c_upVector = glm::cross(c_rightVector, c_direction);

	//this->rotation = c_direction;

	/*cout << c_direction.x;
	cout << "--";
	cout << c_direction.y;
	cout << "--";
	cout << c_direction.z << endl;*/

}
void Camera::Roll() {

	c_rightVector = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		cos(horizontalAngle - 3.14f / 2.0f),
		0
	);

	c_upVector = glm::cross(c_rightVector, c_direction);
}

void Camera::Render(Shader shader) {
	//c_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)1024.0f / (float)768.0f, 0.0001f, 5000.0f);
	
	c_viewMatrix = glm::lookAt(
		c_position, // Camera is at (x,y,z), in World Space
		c_position + c_direction, // and looks at some point
		c_upVector  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "ViewMatrix"), 1, GL_FALSE, &this->c_viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader.GetID(), "ProjectionMatrix"), 1, GL_FALSE, &this->c_projectionMatrix[0][0]);

	UpdateProcess(); 
}

const glm::mat4& Camera::getViewMatrix() const noexcept
{
	return c_viewMatrix;
}

const glm::mat4& Camera::getProjMatrix() const noexcept
{
	return c_projectionMatrix;
}

const glm::mat4& Camera::getProjectionViewMatrix() const noexcept
{
	return c_projViewMatrix;
}

void Camera::keyboard_event(int key, int scancode, int action, int mode) 
{

	if (key == GLFW_KEY_F) {
		//todo camera toogle
	}

	if (action == GLFW_PRESS) {
		key_events_buffer[key] = true;
	}
	else if (action == GLFW_RELEASE) {

		key_events_buffer[key] = false;
	}
}

void Camera::mouse_event(double xpos, double ypos)
{
	Rotate(ypos, xpos);
}

void Camera::UpdateProcess()
{
	if (key_events_buffer[GLFW_KEY_W])
	{
		MoveForward(0.005);
	}
	else if (key_events_buffer[GLFW_KEY_S])
	{
		MoveForward(-0.005);
	}
	else if (key_events_buffer[GLFW_KEY_A])
	{
		MoveHorizontal(-0.005);
	}
	else if (key_events_buffer[GLFW_KEY_D])
	{
		MoveHorizontal(0.005);
	}
	else if (key_events_buffer[GLFW_KEY_I])
	{
		MoveUpward(0.005);
	}
	else if (key_events_buffer[GLFW_KEY_K])
	{
		MoveUpward(-0.005);
	}
	else if (key_events_buffer[GLFW_KEY_UP])
	{
		Rotate(1, 0);
	}
	else if (key_events_buffer[GLFW_KEY_DOWN])
	{
		Rotate(-1, 0);
	}
	else if (key_events_buffer[GLFW_KEY_RIGHT])
	{
		Rotate(0,-1);
	}
	else if (key_events_buffer[GLFW_KEY_LEFT])
	{
		Rotate(0, 1);
	}
}