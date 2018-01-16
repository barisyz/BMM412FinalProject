#include "Animation.h"

Animation::Animation()
{
	mIsAnimated = false;
}

Animation::Animation(const aiScene& scene)
{
	mIsAnimated = true;
}

bool Animation::IsAnimated()
{
	return mIsAnimated;
}

bool Animation::IsLocationSetted()
{
	return mIsLocationSetted;
}

void Animation::SetGlobalInverseTransform(aiMatrix4x4 mat)
{
	mGlobalInverseTransform = aiMatrix4x4();
}

void Animation::SetScene(const aiScene *scene)
{
	mScene = scene;
	mIsAnimated = true;
}

void Animation::BoneTransform(float time, std::vector<aiMatrix4x4> &Transforms)
{
	aiMatrix4x4 identityMatrix = aiMatrix4x4();

	float TicksPerSecond = mScene->mAnimations[0]->mTicksPerSecond != 0 ?
		mScene->mAnimations[0]->mTicksPerSecond : 25.0f;

	float TimeInTicks = time * TicksPerSecond;
	float animationTime = fmod(TimeInTicks, mScene->mAnimations[0]->mDuration);

	if (mHasSplitAnimation)
	{
		animationTime = fmod(TimeInTicks, mDuration);

	}


		ReadNodeHeirarchy(animationTime, mScene->mRootNode, identityMatrix);

		Transforms.resize(mNumBones);

		for (unsigned int i = 0; i < mNumBones; i++) {
			Transforms[i] = mBoneInfo[i].FinalTransformation;
		}
	
}
	//mScene->mRootNode->  pNodeAnim->mPositionKeys[i + 1].mTime


void Animation::MakeBoneTransform(float time)
{
	std::vector<aiMatrix4x4> transforms;

	BoneTransform(time, transforms);

	for (GLuint i = 0; i < transforms.size(); i++) {
		glUniformMatrix4fv(mBoneLocation[i], 1, GL_TRUE, &transforms[i][0][0]);
	}
}

void Animation::AddBoneInfo(unsigned int *boneIndex, aiBone *bone, std::string boneName)
{
	if (mBoneMapping.find(boneName) == mBoneMapping.end())
	{
		// Allocate an index for a new bone
		*boneIndex = mNumBones;
		mNumBones++;
		BoneInfo bi;
		mBoneInfo.push_back(bi);
		mBoneInfo[*boneIndex].BoneOffset = bone->mOffsetMatrix;

		mBoneMapping[boneName] = *boneIndex;
	}
	else
	{
		*boneIndex = mBoneMapping[boneName];
	}
}

void Animation::SetupBonesLocation(GLuint shader)
{
	for (unsigned int i = 0; i < 100; i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		snprintf(Name, sizeof(Name), "Bones[%d]", i);
		mBoneLocation[i] = glGetUniformLocation(shader, Name);
	}

	mIsLocationSetted = true;
}

void Animation::ReadNodeHeirarchy(float time, const aiNode* pNode, const aiMatrix4x4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);
	
	const aiAnimation* pAnimation = mScene->mAnimations[0];

	//unsigned int StartIndex = FindPosition()
	aiMatrix4x4 NodeTransformation(pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) 
	{
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		CalcInterpolatedScaling(Scaling, time, pNodeAnim);
		aiMatrix4x4 ScalingM = aiMatrix4x4();
		ScalingM.a1 = Scaling.x; ScalingM.b2 = Scaling.y; ScalingM.c3 = Scaling.z;

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, time, pNodeAnim);

		aiMatrix4x4 RotationM = aiMatrix4x4(RotationQ.GetMatrix());
		RotationM.d4 = 1.0f;

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		CalcInterpolatedPosition(Translation, time, pNodeAnim);
		aiMatrix4x4 TranslationM = aiMatrix4x4();
		TranslationM.a4 = Translation.x; TranslationM.b4 = Translation.y; TranslationM.c4 = Translation.z;

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM;
	}

	aiMatrix4x4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (mBoneMapping.find(NodeName) != mBoneMapping.end()) {
		unsigned int BoneIndex = mBoneMapping[NodeName];
		mBoneInfo[BoneIndex].FinalTransformation = mGlobalInverseTransform * GlobalTransformation *
			mBoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(time, pNode->mChildren[i], GlobalTransformation);
	}
}

void Animation::CalcInterpolatedPosition(aiVector3D& Out, float time, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = FindPosition(time, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (time - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}


void Animation::CalcInterpolatedRotation(aiQuaternion& Out, float time, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = FindRotation(time, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (time - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}


void Animation::CalcInterpolatedScaling(aiVector3D& Out, float time, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = FindScaling(time, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (time - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

const aiNodeAnim* Animation::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return NULL;
}

unsigned int Animation::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	unsigned int min = mHasSplitAnimation ? mStartKey : 0;
	unsigned int max = mHasSplitAnimation ? mEndKey : pNodeAnim->mNumPositionKeys - 1;

	for (unsigned int i = min; i < max; i++) {
		if (AnimationTime <= (float)pNodeAnim->mPositionKeys[i+1].mTime) {
			return i;
		}
	}

	return 0;
}


unsigned int Animation::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);
	
	unsigned int min = mHasSplitAnimation ? mStartKey : 0;
	unsigned int max = mHasSplitAnimation ? mEndKey : pNodeAnim->mNumRotationKeys - 1;

	for (unsigned int i = min; i < max; i++) {
		if (AnimationTime <= (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}


unsigned int Animation::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);
	unsigned int min = mHasSplitAnimation ? mStartKey : 0;
	unsigned int max = mHasSplitAnimation ? mEndKey : pNodeAnim->mNumScalingKeys - 1;

	for (unsigned int i = min; i < max; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	return 0;
}

void Animation::AddAnimationInfo(std::string name, unsigned int startKey, unsigned int endKey, float duration)
{
	AnimationInfo animationInfo;

	animationInfo.duration = duration;
	animationInfo.startKey = startKey;
	animationInfo.endKey = endKey;
	mHasSplitAnimation = true;
	if (mAnimationInfo.find(name) == mAnimationInfo.end())
	{
		mAnimationLocation.push_back(animationInfo);
		mAnimationInfo.insert(std::pair<std::string, unsigned int>(name, mAnimationLocation.size() - 1));

		if (mAnimationLocation.size() == 1) {
		
			SetAnimation(name);
		}
	}
}

bool Animation::IsAnimatedInThisFrame()
{
	return mIsAnimatedInThisFrame;
}

void Animation::SetAnimation(std::string animationName)
{
	if (mHasSplitAnimation)
	{
		auto index = mAnimationInfo.find(animationName);

		AnimationInfo anim = mAnimationLocation[index->second];

		mStartKey = anim.startKey;
		mEndKey = anim.endKey;
		mDuration = anim.duration;
	}
}

void Animation::StartAnimation()
{
	mIsAnimatedInThisFrame = true;
}

void Animation::StopAnimation()
{
	mIsAnimatedInThisFrame = false;
}

float Animation::GetAnimationDuration()
{
	return mDuration;
}

bool Animation::IsSplitAnimation()
{
	return mHasSplitAnimation;
}
