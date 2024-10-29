#ifndef GRAPHICS_APPLICATION_HPP
#define GRAPHICS_APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../monteCarlo/monteCarlo.hpp"

class Application {
	GLint windowWidth, windowHeight;
	std::string title;
	GLFWwindow* window;
	MonteCarlo monteCarlo;
	bool initiated;
public:
	Application(GLint width, GLint height, const char* title = "") : windowWidth(width), windowHeight(height), title(title), window(nullptr), initiated(false) {
		monteCarlo.setPathPrefix("src/monteCarlo/");
	}

	void createContext();
	void mainLoop();

	~Application() {
		glfwDestroyWindow(window);
		glfwTerminate();
		std::cout << "Destructor called" << std::endl;
	}
};
#endif // !APPLICATION_HPP
