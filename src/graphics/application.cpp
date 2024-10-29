#include "application.hpp"

void Application::createContext(){
	if (!glfwInit()) {
		std::cerr << "GLFW could not be initiated!" << std::endl;
		exit(1);	
	}

	this->window = glfwCreateWindow(this->windowWidth, this->windowHeight, this->title.c_str(), nullptr, nullptr);
	if (this->window == nullptr) {
		std::cerr << "GLFW could not initiate the window context!" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwMakeContextCurrent(this->window);
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cout << glewGetErrorString(err) << std::endl;
		glfwDestroyWindow(this->window);
		glfwTerminate();
		exit(-1);
	}
	glViewport(0, 0, this->windowWidth, this->windowHeight);
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl << "GPU: " << glGetString(GL_RENDERER) << std::endl;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	initiated = true;
}

void Application::mainLoop(){
	if (!initiated) {
		std::cout << "The OpenGL has not been initialized yet." << std::endl;
		return;
	}

	try {
		monteCarlo.readInputFromFile("input/input.txt");
	}
	catch (const std::invalid_argument& ex) {
		std::cerr << ex.what() << std::endl << "Returning..." << std::endl;
	}

	monteCarlo.setupShaders();
	monteCarlo.printBoundaryPoints();
	while (!glfwWindowShouldClose(this->window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		glfwPollEvents();
		glfwSwapBuffers(this->window);
	}
}
