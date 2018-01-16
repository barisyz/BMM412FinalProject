#ifndef ANIMATION_H
#define ANIMATION_H

#include "assimp\scene.h"
#include "assimp\matrix4x4.h"
#include "GL\glew.h"
#include <stdio.h>
#include <vector>
#include <map>
#include <string.h>

#define MAX_BONES 100
#define MAX_ANIMATION 4


class Animation
{
public:
	//Animation(const aiScene* scene);
	Animation();
	Animation(const aiScene & scene);
	//~Animation();
	bool IsAnimated();
	bool IsLocationSetted();
	void BoneTransform(float time, std::vector<aiMatrix4x4>& Transforms);
	void SetGlobalInverseTransform(aiMatrix4x4 mat);
	void SetScene(const aiScene *scene);
	void AddBoneInfo(unsigned int *boneIndex, aiBone *bone, std::string boneName);
	void SetupBonesLocation(GLuint shader);
	void MakeBoneTransform(float time);
	void AddAnimationInfo(std::string name, unsigned int startKey, unsigned int endKey, float duration);

	bool IsAnimatedInThisFrame();

	void SetAnimation(std::string animationName);

	void StartAnimation();

	void StopAnimation();

	float GetAnimationDuration();

	bool IsSplitAnimation();
private:
	struct BoneInfo
	{
		aiMatrix4x4 BoneOffset;
		aiMatrix4x4 FinalTransformation;

		BoneInfo()
		{
			BoneOffset = aiMatrix4x4();
			FinalTransformation = aiMatrix4x4();
		}
	};
	struct AnimationInfo 
	{
		unsigned int startKey;
		unsigned int endKey;
		float duration;

		AnimationInfo()
		{
			startKey = 0;
			endKey = 1;
			duration = 0.0f;
		}
	};
	bool mIsAnimated = false;
	bool mIsLocationSetted = false;
	bool mIsAnimatedInThisFrame = false;
	bool mHasSplitAnimation = false;
	const aiScene *mScene;
	
	aiMatrix4x4 mGlobalInverseTransform;
	std::map<std::string, unsigned int> mBoneMapping;
	GLuint mBoneLocation[MAX_BONES];
	std::vector<BoneInfo> mBoneInfo;
	std::map<std::string, unsigned int> mAnimationInfo;
	std::vector<AnimationInfo> mAnimationLocation;
	unsigned int mCurrentAnimation = 0;
	unsigned int mNumBones = 0;

	void ReadNodeHeirarchy(float time, const aiNode * pNode, const aiMatrix4x4 & ParentTransform);
	void CalcInterpolatedPosition(aiVector3D & Out, float time, const aiNodeAnim * pNodeAnim);
	void CalcInterpolatedRotation(aiQuaternion & Out, float time, const aiNodeAnim * pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D & Out, float time, const aiNodeAnim * pNodeAnim);
	const aiNodeAnim * FindNodeAnim(const aiAnimation * pAnimation, const std::string NodeName);

	unsigned int FindPosition(float AnimationTime, const aiNodeAnim * pNodeAnim);
	unsigned int FindRotation(float AnimationTime, const aiNodeAnim * pNodeAnim);
	unsigned int FindScaling(float AnimationTime, const aiNodeAnim * pNodeAnim);
	
	unsigned int mStartKey = 0;
	unsigned int mEndKey = 1;
	float mDuration = 0.0f;
};

#endif
