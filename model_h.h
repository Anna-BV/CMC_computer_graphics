#ifndef MODEL_H
#define MODEL_H

#include <glad/include/glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh_h.h"
#include "shader_h.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
class Model
{
public:
	vector<Texture> textures_loaded; // список уже загруженных текстур	
	vector<Mesh>    meshes;
	string directory;
	bool gammaCorrection;
	// методы нашего класс
	Model(char* path) { // загрузка модели
		loadModel(path);
	}
	void Draw(Shader goodshader) {
		for (unsigned int i = 0; i < meshes.size(); i++) { // итераци€ по списку полигональных сеток
			meshes[i].Draw(goodshader);                    // и вызов их методов Draw
		}
	}
private:
	void loadModel(string path) {
		Assimp::Importer importer; // создаетс€ экземпл€р класса Importer
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) //  проверка на отличие от нул€ указателей на объект сцены
		{											//  и корневого узла сцены, а также флаг состо€ни€, сообщающий о неполноте возвращенных данных
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// из полного пути к файлу убираем путь к каталогу с ним
		directory = path.substr(0, path.find_last_of('/'));
		// обработка узлов сцены
		processNode(scene->mRootNode, scene);
	}
    void processNode(aiNode* node, const aiScene* scene) {
		// обработка всех полигональных сеток
		for (unsigned int i = 0; i < node->mNumMeshes; i++) {
			//получаем указатель на объект сетки 
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			// преобразуем объект в экземпл€р нашего класса и сохран€ем в списке meshes
			meshes.push_back(processMesh(mesh, scene));
		}
		// обработка дл€ каждого узла потомка
		for (unsigned int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}
};

Mesh processMesh(aiMesh* mesh, const aiScene* scene)
{

	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// обработка координат, нормалей и текстурных координат вершин
		glm::vec3 vector; // положени€ вершин
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// дл€ нормалей 
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		//считывание текстурных координат
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// орбаботка индексов
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// обработка материала
		if (mesh->mMaterialIndex >= 0)
		{
			// получим указатель на объект из массива mMaterials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// загружаем текстуры (диффузионные или зеркального блеска)
			vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

	return Mesh(vertices, indices, textures);

}
vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{   
		// считывание пути к файлу текстуры 
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j <  .size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{		// загрузка файла и генерации текстуры
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			// занесем текстуру в список уже загруженных
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
{
	string filename = string(path);
	filename = directory + '/' + filename;
	// создание идентификатора структуры
	unsigned int textureID;
	glGenTextures(1, &textureID);
	// // nrComponents  количестве цветовых каналов получившегос€ изображени€
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		// св€зывание текстуры
		glBindTexture(GL_TEXTURE_2D, textureID);
		// сгенерируем текстуру
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		// создаем все необходимые мипмап-уровни дл€ текстуры
		glGenerateMipmap(GL_TEXTURE_2D);
		//устанавливаем параметры наложени€
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// параметры фильтрации
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;	
	}
	// освобождение пам€ти
	stbi_image_free(data);
	return textureID;
#endif MODEL_H