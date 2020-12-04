#pragma once

#include <vector>

#include <GLEW/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD, 
    BACKWARD,
    LEFT,
    RIGHT
};

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;

class Camera
{
public:
    // атрибуты камеры
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // углы Эйлера
    GLfloat Yaw;
    GLfloat Pitch;
    // опции камеры
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;

  
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }
    
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // функция возвращает view матрицу, используя функцию glm::lookAt
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
    }

    // функция,нажимая одну из клавиш WASD, положение камеры обновляется в соответствии с её новыми координатами. 
    // вперед или назад, то добавляем или вычитаем вектор направления из вектора положения камеры.
    // при движении в боковом направлении через векторное произведение находим направленный в право вектор
    void Keyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD)
            this->Position -= this->Front * velocity;
        if (direction == LEFT)
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;
    }

    // 
    void MouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= this->MouseSensitivity; // умножение, чтобы движение мыши не были слишком быстрыми
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset;  // добавим смещения к переменным pitch и yaw
        this->Pitch += yoffset;

    // сравниваем значение угла с допустимой величиной,если что меняем результирующее значение 
        if (constrainPitch)
        {
            if (this->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }

        this->updateCameraVectors();
    }
    // 
    // параметр yoffset представляет собой расстояние, прокрученное по вертикали
    void MouseScrolling(GLfloat yoffset)
    {
        if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
            this->Zoom -= yoffset;
        if (this->Zoom <= 1.0f)
            this->Zoom = 1.0f;
        if (this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

private:
    // 
    void updateCameraVectors()
    {
        // вычисление результирующего вектора направления камеры по значениям углов рыскания и тангажа
        glm::vec3 front;
        front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        front.y = sin(glm::radians(this->Pitch));
        front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
        this->Front = glm::normalize(front);
        //  
        this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  
        this->Up = glm::normalize(glm::cross(this->Right, this->Front));
    }
};