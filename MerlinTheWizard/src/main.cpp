#include "Renderer.h"
#include "Window.h"
#include <stdio.h>

#define SCREEN_TITLE "Merlin The Wizard"
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
	glfwSetErrorCallback(error_callback); //Error callback

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

	GLFWwindow *win = window.getWindow();
	Renderer renderer = Renderer(win);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(win)){

		/* Render here */
		renderer.RenderScene();

		/* Swap front and back buffers */
		window.swapBuffers();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}