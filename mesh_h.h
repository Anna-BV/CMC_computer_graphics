#ifndef MESH_H
#define MESH_H
#include <GLAD/include/glad/glad.h> // 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader_h.h"

#include <string>
#include <vector>
using namespace std;
// набор вершин полигональной сетки 
struct Vertex {
    glm::vec3 Position; // вектор позиции
    glm::vec3 Normal;   // вектор нормали
    glm::vec2 TexCoords;// вектор текстурных координат
};
struct Texture {
    unsigned int id; 
    string type; // диффузна€ или бликова€
};
class Mesh {
public:
  
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // функции класса
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }
    // функци€ отрисовки
    void Draw(Shader myShader)
    {
        unsigned int diffuseNr = 1; // диффузна€ 
        unsigned int specularNr = 1; // бликова€
        for (unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // активируем текстурный блок
            // получаем номер текстуры
            stringstream ss;
            string number;
            string name = textures[i].type; // определ€ем тип текстуры
            if (name == "texture_diffuse")
                ss << diffuseNr++; // передаем unsigned int в stream
            else if (name == "texture_specular")
                ss << specularNr++; // передаем unsigned int в stream
            number = ss.str(); // узнаем номер текстурного объекта

            myShader.setFloat(("material." + name + number).c_str(), i); //устанавливаем этому текстурному объекту номер
            glBindTexture(GL_TEXTURE_2D, textures[i].id); // ура св€зываем текстуру
        }
        glActiveTexture(GL_TEXTURE0);

        // отрисовывем полигональную сетку
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
private:
    
    unsigned int VAO, VBO, EBO; // буферы
   // инициализаци€ буферы
    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
            &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
       
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));  //offsetof в качестве первого аргумента - структура, а  второго Ч им€ переменной структуры
                                                                                                           // возвращает смещение в байтах указанной структуры, до переменной во втором аргументе
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }
};

#endif MESH_H