#include "Model.h"

Model::Model()
{
}

Model::Model(const char * modelPath)
{
	Assimp::Importer importer;

	mScene = (importer.ReadFile(modelPath,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_GenSmoothNormals));

	if (!mScene)
	{
		fprintf(stderr, "Error: %s\n", importer.GetErrorString());
	}

	LoadModel(mScene);

	if (mScene->HasAnimations()) {
		mScene = importer.GetOrphanedScene();
		mAnimation.SetScene(mScene);
	}
}

bool Model::LoadModel(const aiScene* scene)
{
	if (scene->HasAnimations())
	{
		aiMatrix4x4 inverseTransformMat = scene->mRootNode->mTransformation;
		inverseTransformMat.Inverse();

		mAnimation.SetGlobalInverseTransform(inverseTransformMat);
	}

	//todo texture icin bakilacak suan embeded material icin yapiyor
	if (scene->HasMaterials())
	{
		float shininess;
		aiColor4D tempColor;
		unsigned int max;
		for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
			sMaterial material;
			material.index = i;
			aiMaterial* mat = scene->mMaterials[i];

			aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &tempColor);
			material.Ka = glm::vec4(tempColor.r, tempColor.b, tempColor.g, tempColor.a);

			aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &tempColor);
			material.Kd = glm::vec4(tempColor.r, tempColor.b, tempColor.g, tempColor.a);

			aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &tempColor);
			material.Ks = glm::vec4(tempColor.r, tempColor.b, tempColor.g, tempColor.a);

			aiGetMaterialColor(mat, AI_MATKEY_COLOR_EMISSIVE, &tempColor);
			material.Ke = glm::vec4(tempColor.r, tempColor.b, tempColor.g, tempColor.a);

			aiGetMaterialFloatArray(mat, AI_MATKEY_SHININESS, &shininess, &max);
			material.Ns = shininess / 4.0;

			mMaterials.push_back(material);
		}
	}

	ProcessNode(scene->mRootNode, scene);
	return true;
}

void Model::ApplyMaterial(unsigned int materialIndex, Shader shader)
{
	sMaterial mat = mMaterials[materialIndex];

	glUniform4fv(glGetUniformLocation(shader.GetID(), "Ka"), 1, &mat.Ka[0]);
	glUniform4fv(glGetUniformLocation(shader.GetID(), "Ks"), 1, &mat.Ks[0]);
	glUniform4fv(glGetUniformLocation(shader.GetID(), "Kd"), 1, &mat.Kd[0]);
	glUniform1f(glGetUniformLocation(shader.GetID(), "Shininess"), mat.Ns);
}

void Model::Draw(Shader shader, float time)
{
	if (mAnimation.IsAnimated())
	{
		if (!mAnimation.IsLocationSetted())
		{
			mAnimation.SetupBonesLocation(shader.GetID());
		}
		float RunningTime = (float)((double)glfwGetTime() - (double)time);
		mAnimation.MakeBoneTransform(RunningTime);
		
	}

	for (unsigned int i = 0; i < mMeshes.size(); i++)
	{
		ApplyMaterial(mMeshes[i].GetMaterialIndex(), shader);
		mMeshes[i].Draw(shader);
	}
}

bool Model::IsAnimated()
{
	return mAnimation.IsAnimated();
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Mesh m = Model::ProcessMesh(mesh, scene);
		m.SetMaterialIndex(mesh->mMaterialIndex);
		mMeshes.push_back(m);
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<VertexBoneData> bones;

	// Walk through each of the mesh's vertices
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;

		// Normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		if (mesh->mTextureCoords[0]) 
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mNumBones > 0) 
	{
		bones.resize(mesh->mNumVertices);

		for (GLuint i = 0; i < mesh->mNumBones; i++) {
			std::string BoneName(mesh->mBones[i]->mName.data);
			GLuint BoneIndex = 0;

			mAnimation.AddBoneInfo(&BoneIndex, mesh->mBones[i], BoneName);

			for (GLuint j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
				GLuint VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float Weight = mesh->mBones[i]->mWeights[j].mWeight;

				for (GLuint k = 0; k < NUM_BONES_PER_VERTEX; k++) {
					bones[VertexID].IDs[k] = BoneIndex;
					bones[VertexID].Weights[k] = Weight;
				}
			}
		}
	}

	return Mesh(vertices, indices, bones);
}

