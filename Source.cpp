#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
void processInput(GLFWwindow* window) { // обработка событий вводы
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
   
    // Инициализируем GLFW и конфигурирование 
    if (!glfwInit()) {
        fprintf(stderr, "Îøèáêà ïðè èíèöèàëèçàöèè GLFWn");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window;
    // Открыть окно и создать в нем контекст OpenGL
    window = glfwCreateWindow(640, 480, "CMC_computer_graphics", NULL, NULL);
    if (!window)
    {
        fprintf(stderr,"Íåâîçìîæíî îòêðûòü îêíî");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
     // glad: загрузка указателей на OpenGL функции
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr, "Failed to initialize GLAD");
        return -1;
    }
   glViewport(0,0,640,840); 
   while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
   }
    
   
    /*while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
       
        // ëåâàÿ ãðàíü 
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        // ïðàâàÿ ãðàíü
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f,-0.5f);
        // íèæíÿÿ ãðàíü
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // âåðõíÿÿ ãðàíü
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        // çàäíÿÿ ãðàíü
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // ïåðåäíÿÿ ãðàíü        
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glEnd();
        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f(0.0f, 0.5f);
        //glVertex2f(0.5f, -0.5f);
        //glEnd();

        // Ñáðàñûâàåì áóôåðû
        glfwSwapBuffers(window);
        glfwPollEvents();
    } */

    glfwTerminate();
    return 0;
}
