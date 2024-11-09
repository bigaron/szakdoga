#ifndef GRAPHICS_APPLICATION_HPP
#define GRAPHICS_APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../monteCarlo/monteCarlo.hpp"
#include "../helperFunctions/helperFunctions.hpp"

class Application {
	GLint windowWidth, windowHeight;
	std::string title;
	GLFWwindow* window;
	MonteCarlo monteCarlo;
	bool initiated;
public:
	Application(GLint width, GLint height, const char* title = "") : windowWidth(width), windowHeight(height), title(title), window(nullptr), initiated(false) {
		monteCarlo.setPathPrefix("src/monteCarlo/");
		createContext();
		monteCarlo.setupMonteCarlo({ -1, 1.0f, 3, 20}, 720, 1280);
	}

	void createContext();
	void mainLoop();

	void generateBezierToFile(std::string fileSrc, const std::vector<glm::vec4> contP, float gran);

	~Application() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
	}
};
#endif // !APPLICATION_HPP
