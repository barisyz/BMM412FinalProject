#include "Renderer.h"
#include "Window.h"
#include <stdio.h>

#define SCREEN_TITLE "Merlin The Wizard"
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{

	glfwSetErrorCallback(error_callback); //Error callback

	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	Window window = Window(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

	// Accept fragment if it closer to the camera than the former one
	//glDepthFunc(GL_LESS);

	GLFWwindow *win = window.getWindow();
	Renderer renderer = Renderer(win);

	// Hide the mouse and enable unlimited movement
	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(win, cursor);
	//glfwSetInputMode(win, GLFW_CURSOR, GLFW_CROSSHAIR_CURSOR);

	int width, height;
	double startTime = glfwGetTime();
	double lastTime = 0;
	/* Loop until the user closes the window */	

	while (!glfwWindowShouldClose(win)) {
		//Get the time betwwen frames
		double currentTime = glfwGetTime();
		double delta = currentTime - lastTime;
		lastTime = currentTime;
		//Set cursor position to middle
		glfwGetWindowSize(win, &width, &height);
		glfwSetCursorPos(win, width / 2, height / 2);

		/* Render here */
		renderer.RenderScene(delta);

		/* Swap front and back buffers */
		window.swapBuffers();

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}