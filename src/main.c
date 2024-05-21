#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "quad_renderer.h"

static GLFWwindow *mWindow;

static void resizeCallback(GLFWwindow *window, int32_t width, int32_t height) {
    leoQuadRendererResize(width, height);
}

int main(int argc, char** argv) {

    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    mWindow = glfwCreateWindow(1000u, 556u, "Quads", NULL, NULL);
    if(!mWindow) {
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(mWindow, &resizeCallback);


    if(!leoQuadRendererInit()) {
        exit(EXIT_FAILURE);
    }

    leoQuadRendererResize(1000u, 556u);


    while (!glfwWindowShouldClose(mWindow))
    {

        leoQuadRendererBegin();
        {
            leoQuadRendererDrawQuad(  0u,   0u, 300u, 300u, 0xffu);
            leoQuadRendererDrawQuad(300u, 300u, 200u, 100u, 0xffffu);
            leoQuadRendererDrawQuad(500u,   0u, 200u, 300u, 0xff0000u);
            leoQuadRendererDrawQuad(10u,   10u, 100u, 100u, 0xffffffu);
        }
        leoQuadRendererEnd();


        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    

    leoQuadRendererDispose();
    glfwTerminate();

    return 0;
}
