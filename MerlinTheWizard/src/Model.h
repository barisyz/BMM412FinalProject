#pragma once
#include "Mesh.h"
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"

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
	Model(const char* modelPath);
	~Model();
	void ApplyMaterial(unsigned int materialIndex, Shader shader);
	void Draw(Shader shader);

private:
	bool LoadModel(const char* path);

	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh * mesh, const aiScene * scene);

	vector<Mesh> mMeshes;
	vector<sMaterial> mMaterials;
};
