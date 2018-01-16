#include "Camera.h"

using namespace std;

Camera::Camera()
{
}

Camera::Camera(int width, int height)
{
	c_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.13f, 2000.0f);



	//example start state
	c_position = { -0.0238649566, 1.06074810, -1.73629391 };
	c_direction = { 0.0262371451, -0.484682769, 0.874296427 };
	c_upVector = { 0.0149242813, 0.874689758, 0.484452963 };
	c_rightVector = { -0.999525845, 0, 0.0307918545 };

	verticalAngle = -0.506000459;
	horizontalAngle = 0.0300004352;

	pointCollider.initialized = true;
	pointCollider.position = ((c_position + c_direction) + c_position) / 2;

	pointCollider.size.x = abs(c_direction.x) * sqrtf((pointCollider.position.x - c_position.x) * (pointCollider.position.x - c_position.x));
	pointCollider.size.y = abs(c_direction.y) * sqrtf((pointCollider.position.y - c_position.y) * (pointCollider.position.y - c_position.y));
	pointCollider.size.z = abs(c_direction.z) * sqrtf((pointCollider.position.z - c_position.z) * (pointCollider.position.z - c_position.z));
}

void Camera::AttachPlayer(Player* player)
{
	mPlayer = player;
}

void Camera::FollowPlayer()
{
	c_position = mPlayer->GetPosition() - c_offset;

	c_direction = { 0, 0, 1 };
	c_upVector = { 0, 1, 0 };
	c_rightVector = { -1, 0, 0 };
}

Player* Camera::GetPlayer()
{
	return mPlayer;
}

glm::vec3 Camera::GetPosition()
{
	return c_position;
}

void Camera::SetPosition(glm::vec3 position)
{
	c_position = position;
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

	glm::vec3 temp = c_direction * direction / 5.0f;
	Movement(temp);
}
void Camera::MoveHorizontal(float direction) {

	glm::vec3 temp = c_rightVector * direction / 5.0f;
	Movement(temp);
}
void Camera::MoveUpward(float direction) {
	if (mFreeMode)
	{
		glm::vec3 temp = c_upVector * direction / 5.0f;
		Movement(temp);
	}
}
void Camera::Movement(glm::vec3 temp) {

	glm::vec3 temp2 = c_position + temp;

	if (!mFreeMode)
	{
		if (!mPlayer->IsPlayerCastingSkill())
		{
			if (temp2.y < 5 && temp2.y > 0 && sqrt(temp2.x * temp2.x + temp2.z * temp2.z) < 3.75) {
				temp.y = 0.0;
				c_position += temp;
			}

			mPlayer->Move(c_position + c_offset);
		}		
	}
	else
	{
		//if (temp2.y < 5 && temp2.y > 0 && sqrt(temp2.x * temp2.x + temp2.z * temp2.z) < 3.75)
		c_position += temp;
	}

}
void Camera::Rotate(float ypos, float xpos) {
	horizontalAngle += sensitivity * xpos;
	verticalAngle += sensitivity * ypos;

	glm::vec3 direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 rightVector = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 upVector = glm::cross(c_rightVector, c_direction);

	if (!mFreeMode) {
		//c_direction.z = 0;

		if (verticalAngle > 0.7)
			verticalAngle = 0.7;
		
		if (verticalAngle < -0.7)
			verticalAngle = -0.7;

		c_direction = direction;
		c_rightVector = rightVector;
		c_upVector = glm::cross(c_rightVector, c_direction);

		glm::vec3 rotation = glm::vec3(0, c_viewMatrix[1][1], 0);
		mPlayer->Rotate(rotation, horizontalAngle);
		
		//c_offset.z = c_offset.z * cos(horizontalAngle - 3.14f / 2.0f);

	}
	else
	{
		c_direction = direction;

		c_rightVector = rightVector;

		c_upVector = upVector;
	}

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

void Camera::Render(GLuint shader, double deltatime) {
	//c_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)1024.0f / (float)768.0f, 0.0001f, 5000.0f);

	c_viewMatrix = glm::lookAt(
		c_position, // Camera is at (x,y,z), in World Space
		c_position + c_direction, // and looks at some point
		c_upVector  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glUniformMatrix4fv(glGetUniformLocation(shader, "ViewMatrix"), 1, GL_FALSE, &this->c_viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "ProjectionMatrix"), 1, GL_FALSE, &this->c_projectionMatrix[0][0]);

	UpdateProcess(deltatime);
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

bool Camera::IsInFreeMode()
{
	return mFreeMode;
}

void Camera::ToogleCamera()
{
	mFreeMode = !mFreeMode;
	mSpeedConstant = mFreeMode ? 5.0f : 1.0f;
	if (!mFreeMode) {
		FollowPlayer();
	}
}

void Camera::keyboard_event(int key, int scancode, int action, int mode)
{

	if (action == GLFW_PRESS) {
		key_events_buffer[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		key_events_buffer[key] = false;
	}
}

void Camera::mouse_event(double xpos, double ypos)
{
	mouse_position_buffer[0] = xpos;
	mouse_position_buffer[1] = ypos;
	mouse_position_buffer[2] = 1;
}

void Camera::UpdateProcess(double deltatime)
{
	float speed = mSpeedConstant * deltatime;

	if (key_events_buffer[GLFW_KEY_W])
	{
		MoveForward(speed);
	}
	else if (key_events_buffer[GLFW_KEY_S])
	{
		MoveForward(-speed);
	}
	else if (key_events_buffer[GLFW_KEY_A])
	{
		MoveHorizontal(-speed);
	}
	else if (key_events_buffer[GLFW_KEY_D])
	{
		MoveHorizontal(speed);
	}
	else if (key_events_buffer[GLFW_KEY_I])
	{
		MoveUpward(speed);
	}
	else if (key_events_buffer[GLFW_KEY_K])
	{
		MoveUpward(-speed);
	}
	if (mouse_position_buffer[2] == 1) {
		Rotate(mouse_position_buffer[1], mouse_position_buffer[0]);
		mouse_position_buffer[2] = 0;
	}
	pointCollider.position = ((c_position + c_direction) + c_position) / 2;

	pointCollider.size.x = abs(c_direction.x) * sqrtf((pointCollider.position.x - c_position.x) * (pointCollider.position.x - c_position.x));
	pointCollider.size.y = abs(c_direction.y) * sqrtf((pointCollider.position.y - c_position.y) * (pointCollider.position.y - c_position.y));
	pointCollider.size.z = abs(c_direction.z) * sqrtf((pointCollider.position.z - c_position.z) * (pointCollider.position.z - c_position.z));
}

void Camera::SetVelocitySpell() {

	this->spell->SetVelocity(normalize(c_direction) * 2);

}
void Camera::Reshape(float w, float h) {

	c_projectionMatrix = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.13f, 2000.0f);

}