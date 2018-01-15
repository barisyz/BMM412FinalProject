#include "glm/ext.hpp"
#include "stdio.h"
#include <iostream>
#include <string>
#include <sstream>
#include "Player.h"

class Camera
{
public:
	Camera();
	Camera(int width, int height);
	void AttachPlayer(Entity* player);
	void FollowPlayer();
	Entity* GetPlayer();
	glm::vec3 c_position;
	glm::vec3 c_direction;
	glm::vec3 c_rightVector;
	glm::vec3 c_upVector;
	glm::vec3 c_offset = glm::vec3(0.01, -0.15, -0.07);
	float sensitivity = 0.0010f;
	glm::vec3 GetPosition();
	void SetPosition(glm::vec3 position);
	//Move commands
	void MoveTo(glm::vec3 location);
	void MoveForward(float direction);
	void MoveHorizontal(float direction);
	void MoveUpward(float direction);
	void MoveX(float direction);
	void MoveY(float direction);
	void MoveZ(float direction);

	void Rotate(float verticalAngle, float horizontalAngle);
	void Render(GLuint shader, double deltatime);
	void Roll();

	void keyboard_event(int key, int scancode, int action, int mode);
	void mouse_event(double xpos, double ypos);

	void UpdateProcess(double deltatime);

	const glm::mat4& getViewMatrix()           const noexcept;
	const glm::mat4& getProjMatrix()           const noexcept;
	const glm::mat4& getProjectionViewMatrix() const noexcept;

	bool IsInFreeMode();
	void ToogleCamera();
	//void scroll_event(double xoffset, double yoffset);
private:
	Entity* mPlayer;
	glm::mat4 c_projectionMatrix;
	glm::mat4 c_viewMatrix;
	glm::mat4 c_projViewMatrix;
	bool mFreeMode = true;
	bool key_events_buffer[1024] = { 0 };
	double mouse_position_buffer[3];

	float horizontalAngle, verticalAngle;
	
	bool Condition();
	void Movement(glm::vec3 temp);
};