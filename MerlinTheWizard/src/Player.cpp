#include "Player.h"

Player::Player()
{

}

Player::Player(const char * modelPath, Shader shader) : Entity(modelPath, shader)
{

}

Player::~Player()
{
}

void Player::keyboard_event(int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS) {
		key_events_buffer[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		key_events_buffer[key] = false;
	}
}

void Player::Move(glm::vec3 pos)
{
	Translate(pos);
	SetPosition(pos);
}

void Player::Update()
{
}



