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
	GetModelPointer()->GetAnimationPointer()->SetAnimation("walk");
	GetModelPointer()->GetAnimationPointer()->StartAnimation();
}

bool Player::IsPlayerCastingSkill()
{
	return mIsPlayerCastingSkill;
}

void Player::CastSkill()
{
	if (!mIsPlayerCastingSkill)
	{
		Animation *anim = GetModelPointer()->GetAnimationPointer();
		mIsPlayerCastingSkill = true;
		anim->SetAnimation("skillCast");
		mAnimDuration = anim->GetAnimationDuration();
		GetModelPointer()->GetAnimationPointer()->StartAnimation();
	}
	
}

void Player::Update(float deltaTime)
{
	if (mAnimDuration > 0)
	{
		mAnimDuration -= deltaTime;
		if (mAnimDuration <= 0)
		{
			mIsPlayerCastingSkill = false;
			GetModelPointer()->GetAnimationPointer()->StopAnimation();
			mAnimDuration = 0.0f;
		}
	}
}



