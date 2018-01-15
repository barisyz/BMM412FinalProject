#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<VertexBoneData> bones)
{
	mVertices = vertices;
	mIndices = indices;
	mBones = bones;

	SetupMesh();
}

Mesh::~Mesh()
{
}


void Mesh::Draw(Shader shader)
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, mIndices.size());
	glBindVertexArray(0);
}

void Mesh::SetMaterialIndex(unsigned int materialIndex)
{
	mMaterialIndex = materialIndex;
}

unsigned int Mesh::GetMaterialIndex()
{
	return mMaterialIndex;
}

void Mesh::SetupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	if (mBones.size() <= 0)
	{
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex) + sizeof(VertexBoneData) * mBones.size(), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mVertices.size() * sizeof(Vertex), &mVertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), sizeof(VertexBoneData) * mBones.size(), &mBones[0]);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);

	if (mBones.size() > 0)
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

		glEnableVertexAttribArray(3);
		glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid *)(mVertices.size() * sizeof(Vertex)));

		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(mVertices.size() * sizeof(Vertex) + offsetof(VertexBoneData, Weights)));

		/*std::cout << "sizeof(Vertex):" << sizeof(Vertex) << "\n";
		std::cout << "(GLvoid *)offsetof(Vertex, Normal)" << (GLvoid *)offsetof(Vertex, Normal) << "\n";
		std::cout << "(GLvoid *)offsetof(Vertex, TexCoords)" << (GLvoid *)offsetof(Vertex, TexCoords) << "\n";
		std::cout << "sizeof(VertexBoneData):" << sizeof(VertexBoneData) << "\n";
		std::cout << "(const GLvoid*)sizeof(Vertex):" << (const GLvoid*)sizeof(Vertex) << "\n";
		std::cout << "(const GLvoid*)(sizeof(Vertex) + 16):" << (const GLvoid*)(sizeof(Vertex) + 16) << "\n";*/
	}
	else
	{
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));
	}
	glBindVertexArray(0);
}

//
//void Mesh::LoadMesh(const std::string & Filename)
//{
//	if (!Mesh::LoadObjFile(Filename)) {
//		throw::std::runtime_error("Cannot load obj file!\n");
//	}
//
//	unsigned int offset = 0;
//	
//	glGenVertexArrays(1, &mVao);
//	glBindVertexArray(mVao);
//
//	// Create and initialize a offset object
//	glGenBuffers(1, &mVbo);
//	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
//	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3) + mNormals.size() * sizeof(glm::vec3), NULL, mDrawType);
//	glBufferSubData(GL_ARRAY_BUFFER, offset, mVertices.size() * sizeof(glm::vec3), &mVertices[0]);
//	offset += mVertices.size() * sizeof(glm::vec3);
//
//	if (mNormals.size() > 0) 
//	{
//		glBufferSubData(GL_ARRAY_BUFFER, offset, mNormals.size() * sizeof(glm::vec3), &mNormals[0]);
//		offset += mNormals.size() * sizeof(glm::vec3);
//	}
//
//	GLuint vPosition = glGetAttribLocation(mShader, "vPosition");
//	glEnableVertexAttribArray(vPosition);
//	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	GLuint vNormal = glGetAttribLocation(mShader, "vNormal");
//	glEnableVertexAttribArray(vNormal);
//	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(mVertexCount));
//
//	glBindVertexArray(0);
//}
//
//bool Mesh::LoadObjFile(const std::string & Filename)
//{
//	std::vector< unsigned int > vertexIndices, texIndices, normalIndices;
//	std::vector< glm::vec3 > tempVertices;
//	std::vector< glm::vec3 > tempNormals;
//	std::vector< glm::vec2 > tempTexCoords;
//
//	FILE * file = fopen(Filename.c_str(), "r");
//
//	if (file == NULL) {
//		std::cout << Filename.c_str() << " can not opened!\n";
//		return false;
//	}
//
//	int vertexCounter = 0;
//	int index = 0; //current verticesArr index
//	const int max_line = 128;
//	char line[max_line];
//
//	bool hasVertexNormal = false;
//	bool hasTextureCoords = false;
//
//	while (0 != fgets(line, max_line, file))
//	{
//		char what[max_line];
//		what[0] = 0; /* empty */
//		sscanf(line, "%s", what); /* get the first thing on the line */
//		if (0 == strcmp(what, "v"))
//		{ /* vertex coordinates */
//			GLfloat x, y, z;
//			if (4 != sscanf(line, "%s%f%f%f", what, &x, &y, &z)) {
//				std::cout << "Parse error on 'v' line!\n";
//				return false;
//			}
//
//			//TODO vertices
//			tempVertices.push_back(glm::vec3(x, y, z));
//		}
//		else if (0 == strcmp(what, "vn"))
//		{
//			GLfloat a, b, c, n;
//			if (4 < sscanf(line, "%s%f%f%f", what, &a, &b, &c)) {
//				std::cout << "Parse error on 'vn' line!\n";
//				return false;
//			}
//
//			hasVertexNormal = true;
//			tempNormals.push_back(glm::vec3(a, b, c));
//		}
//		else if (0 == strcmp(what, "vt"))
//		{
//			GLfloat a, b, n;
//			if (3 < sscanf(line, "%s%f%f", what, &a, &b)) {
//				std::cout << "Parse error on 'vt' line!\n";
//				return false;
//			}
//
//			hasTextureCoords = true;
//			tempTexCoords.push_back(glm::vec2(a, b));
//		}
//		else if (0 == strcmp(what, "f"))
//		{
//			unsigned int a[3], b[3], c[3], n; /* 1-based vertex numbers */
//			
//			if (hasVertexNormal && hasTextureCoords) 
//			{
//				if (10 < sscanf(line, "%s%d//%d//%d %d//%d//%d %d//%d//%d", what, &a[0], &b[0], &c[0], &a[1], &b[1], &c[1], &a[2], &b[2], &c[2])) {
//					std::cout << "Parse error on 'vertex normal texture index' line!";
//					return false;
//				}
//
//				vertexIndices.push_back(a[0]); vertexIndices.push_back(a[1]); vertexIndices.push_back(a[2]);
//				texIndices.push_back(b[0]); texIndices.push_back(b[1]); texIndices.push_back(b[2]);
//				normalIndices.push_back(c[0]); normalIndices.push_back(c[1]); normalIndices.push_back(c[2]);
//			}
//			else if(hasVertexNormal && !hasTextureCoords)
//			{
//				if (7 < sscanf(line, "%s%d//%d %d//%d %d//%d", what, &a[0], &b[0], &a[1], &b[1], &a[2], &b[2]))
//				{
//					std::cout << "Parse error on 'vertex normal index' line!\n";
//					return false;
//				}
//
//				vertexIndices.push_back(a[0]); vertexIndices.push_back(a[1]); vertexIndices.push_back(a[2]);
//				normalIndices.push_back(b[0]); normalIndices.push_back(b[1]); normalIndices.push_back(b[2]);
//			}
//			else 
//			{
//				if (7 < sscanf(line, "%s%d//%d %d//%d %d//%d", what, &a[0], &b[0], &a[1], &b[1], &a[2], &b[2])) 
//				{
//					std::cout << "Parse error on 'vertex texture index' line!";
//					return false;
//				}
//
//				vertexIndices.push_back(a[0]); vertexIndices.push_back(a[1]); vertexIndices.push_back(a[2]);
//				texIndices.push_back(b[0]); texIndices.push_back(b[1]); texIndices.push_back(b[2]);
//			}
//		}
//	}
//
//	fclose(file);
//
//	for (unsigned int i = 0; i < vertexIndices.size(); i++)
//	{
//		mVertices.push_back(tempVertices[vertexIndices[i] - 1]);
//	}
//
//	for (unsigned int i = 0; i < normalIndices.size(); i++)
//	{
//		mNormals.push_back(tempNormals[normalIndices[i] - 1]);
//	}
//
//	for (unsigned int i = 0; i < texIndices.size(); i++)
//	{
//		mTexcoords.push_back(tempTexCoords[texIndices[i] - 1]);
//	}
//
//	mVertexCount = mVertices.size() * sizeof(glm::vec3);
//	return true;
//}

