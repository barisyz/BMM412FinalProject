#pragma once
#include "Mesh.h"
#include "Animation.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include <map>

struct sMaterial
{
	unsigned int index;
	double Ns;
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
	glm::vec4 Ke;
	double Ni;
	double d;
	unsigned int illum;
};

class Model
{
public:
	Model();
	Model(const char * modelPath);
	bool LoadModel(const aiScene* scene);
	//~Model();
	void ApplyMaterial(unsigned int materialIndex, Shader shader);
	void Draw(Shader shader, float time);
	bool IsAnimated();
private:
	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);
	Animation mAnimation;
	vector<Mesh> mMeshes;
	vector<sMaterial> mMaterials;

	const aiScene* mScene;
	
};
