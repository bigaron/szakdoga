#ifndef GRAPHICS_APPLICATION_HPP
#define GRAPHICS_APPLICATION_HPP

#include <iostream>
#include <string>
#include <exception>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include "../monteCarlo/monteCarlo.hpp"
#include "../helperFunctions/helperFunctions.hpp"
#include "../monteCarlo/GLFWCallBackHelper.hpp"


class Application {
	GLint windowWidth, windowHeight;
	std::string title;
	GLFWwindow* window;
	MonteCarlo monteCarlo;
	bool initiated;
	double xCoord, yCoord;
	
	bool shouldDebug = false;

	void configureImGui();
public:
	Application(GLint width, GLint height, const char* title = "") : windowWidth(width), windowHeight(height), title(title), window(nullptr), initiated(false) {
		xCoord = yCoord = -1.0;
		monteCarlo.setPathPrefix("src/monteCarlo/");
		createContext();
		monteCarlo.setupMonteCarlo({ -1, 1.f, 6, 40}, height, width);
	}

	void createContext();
	void mainLoop();

	void generateBezierToFile(std::string fileSrc, const std::vector<glm::vec4>& contP, float gran);
	void generateLineToFile(std::string fileSrc, const glm::vec4& startP, const glm::vec4& endp, float gran, const glm::vec4& col);
	

	~Application() {
		glfwDestroyWindow(this->window);
		glfwTerminate();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};
#endif // !APPLICATION_HPP
