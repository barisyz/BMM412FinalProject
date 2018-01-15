#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Player : public Entity
{
public:
	Player();
	Player(const char * modelPath, Shader shader);
	~Player();

	void keyboard_event(int key, int scancode, int action, int mode);
	void Move(glm::vec3 pos);
	void Update();
private:
	bool key_events_buffer[1024] = { 0 };
};
#endif // !PLAYER_H

