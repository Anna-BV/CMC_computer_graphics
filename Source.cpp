#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


int main(void)
{
    GLFWwindow* window;

    // »нициализируем GLFW
    if (!glfwInit()) {
        fprintf(stderr, "ќшибка при инициализации GLFWn");
        return -1;
    }
    // ќткрыть окно и создать в нем контекст OpenGL
    window = glfwCreateWindow(640, 480, "CMC_computer_graphics", NULL, NULL);
    if (!window)
    {
        fprintf(stderr,"Ќевозможно открыть окно");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
   
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.6f, 0.0f, 0.9f, 1.0f);
        glBegin(GL_QUADS);
       
        // лева€ грань 
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        // права€ грань
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f,-0.5f);
        // нижн€€ грань
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(-0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, 0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        // верхн€€ грань
        glVertex3f(-0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, 0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        // задн€€ грань
        glVertex3f(-0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, -0.5f, -0.5f);
        glVertex3f(0.5f, 0.5f, -0.5f);
        glVertex3f(-0.5f, 0.5f, -0.5f);
        // передн€€ грань        
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

        // —брасываем буферы
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}