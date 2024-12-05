#include "application.hpp"

//Not good global variable solution
mouseButtonPosCallBack btnCallBack;

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		btnCallBack.xCoord = x;
		btnCallBack.yCoord  = y;
		btnCallBack.coordsRead = false;
	}
}

void Application::readFromFile(std::string fileSrc) {
	monteCarlo.readInputFromFile(fileSrc.c_str());
}

void Application::configureImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Simulation params");

	if (ImGui::Checkbox("See BVH Outline", &shouldDebug)) {
		monteCarlo.reset();
		monteCarlo.setDebugMode(shouldDebug);
	}
	if (ImGui::Checkbox("Pause", &paused)) {
		monteCarlo.setPaused(paused);
	}

	ImGui::InputInt("Depth", &bvhDepth);
	monteCarlo.setDepth(bvhDepth);


	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}



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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->window, true);
	ImGui_ImplOpenGL3_Init();

//	glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
}



void Application::mainLoop(){
	if (!initiated) {
		std::cout << "The OpenGL has not been initialized yet." << std::endl;
		return;
	}
	bool firstRun = true;
	std::chrono::system_clock::time_point start;
	try {
		//monteCarlo.readInputFromFile("input/generated.txt");
		//monteCarlo.readInputFromFile("input/generated2.txt");

		//diameter -> 60 -> 30 
		//1280 -> 640
		//732 -> 366
		//generateLineToFile("input/square1.txt", glm::vec4(610, 336, 0, 1), glm::vec4(670, 336, 0, 1), 0.01f, glm::vec4(1,1,1,1));
		//generateLineToFile("input/square2.txt", glm::vec4(670, 336, 0, 1), glm::vec4(670, 396, 0, 1), 0.01f, glm::vec4(1, 1, 1, 1));
		//generateLineToFile("input/square3.txt", glm::vec4(670, 396, 0, 1), glm::vec4(610, 396, 0, 1), 0.01f, glm::vec4(1, 1, 1, 1));
		//generateLineToFile("input/square4.txt", glm::vec4(610, 396, 0, 1), glm::vec4(610, 336, 0, 1), 0.01f, glm::vec4(1, 1, 1, 1));

		//generateLineToFile("input/square5.txt", glm::vec4(609, 335, 0, 1), glm::vec4(671, 335, 0, 1), 0.01f, glm::vec4(0, 1, 0, 1));
		//generateLineToFile("input/square6.txt", glm::vec4(671, 335, 0, 1), glm::vec4(671, 397, 0, 1), 0.01f, glm::vec4(0, 1, 0, 1));
		//generateLineToFile("input/square7.txt", glm::vec4(671, 397, 0, 1), glm::vec4(609, 397, 0, 1), 0.01f, glm::vec4(0, 1, 0, 1));
		//generateLineToFile("input/square8.txt", glm::vec4(609, 397, 0, 1), glm::vec4(609, 337, 0, 1), 0.01f, glm::vec4(0, 1, 0, 1));

		//monteCarlo.readInputFromFile("input/square1.txt");
		//monteCarlo.readInputFromFile("input/square2.txt");
		//monteCarlo.readInputFromFile("input/square3.txt");
		//monteCarlo.readInputFromFile("input/square4.txt");
		//monteCarlo.readInputFromFile("input/square5.txt");
		//monteCarlo.readInputFromFile("input/square6.txt");
		//monteCarlo.readInputFromFile("input/square7.txt");
		//monteCarlo.readInputFromFile("input/square8.txt");


		//monteCarlo.readInputFromFile("input/line.txt");
		//monteCarlo.readInputFromFile("input/line2.txt");
		//monteCarlo.readInputFromFile("input/line3.txt");

		//generateLineToFile("input/triangle1.txt", glm::vec4(300, 150, 0, 1), glm::vec4(450, 150, 0, 1), 0.005f, glm::vec4(1, 0, 0, 1));
		//generateLineToFile("input/triangle2.txt", glm::vec4(300, 151, 0, 1), glm::vec4(375, 255, 0, 1), 0.005f, glm::vec4(0, 1, 0, 1));
		//generateLineToFile("input/triangle3.txt", glm::vec4(376, 256, 0, 1), glm::vec4(449, 149, 0, 1), 0.005f, glm::vec4(0, 0, 1, 1));

		//generateLineToFile("input/triangleAndPoint4.txt", glm::vec4(600, 600, 0, 1), glm::vec4(600, 600, 0, 1), 0.5f, glm::vec4(0, 0, 0, 1));
		//generateLineToFile("input/triangleAndPoint.txt", glm::vec4(300, 150, 0, 1), glm::vec4(450, 150, 0, 1), 0.005f, glm::vec4(1, 1, 0, 1));
		//generateLineToFile("input/triangleAndPoint2.txt", glm::vec4(300, 151, 0, 1), glm::vec4(375, 255, 0, 1), 0.005f, glm::vec4(1, 1, 0, 1));
		//generateLineToFile("input/triangleAndPoint3.txt", glm::vec4(376, 256, 0, 1), glm::vec4(449, 149, 0, 1), 0.005f, glm::vec4(1, 1, 0, 1));


		generateLineToFile("input/fourDividers1.txt", glm::vec4(100, 10, 0, 1), glm::vec4(10,100, 0, 1), 0.0125f, glm::vec4(0.2, 0.6, 0.8, 1));
		generateLineToFile("input/fourDividers2.txt", glm::vec4(600, 10, 0, 1), glm::vec4(10, 600, 0, 1), 0.0125f, glm::vec4(0.6, 0.2, 0.8, 1));
		generateLineToFile("input/fourDividers3.txt", glm::vec4(700, 10, 0, 1), glm::vec4(600, 700, 0, 1), 0.0125f, glm::vec4(0.8, 0.2, 0.6, 1));
		generateLineToFile("input/fourDividers4.txt", glm::vec4(1200, 300, 0, 1), glm::vec4(800, 700, 0, 1), 0.0125f, glm::vec4(0.6, 0.8, 0.2, 1));

		//monteCarlo.readInputFromFile("input/fourDividers1.txt");
		//monteCarlo.readInputFromFile("input/fourDividers2.txt");
		//monteCarlo.readInputFromFile("input/fourDividers3.txt");
		//monteCarlo.readInputFromFile("input/fourDividers4.txt");



		//generateBezierToFile("input/bezierAndLine1.txt", {glm::vec4(640, 100, 0, 1),glm::vec4(700, 244, 0, 1), glm::vec4(700, 488, 0, 1), glm::vec4(320, 488, 0, 1)}, 0.001f, glm::vec4(0, 1, 0, 1));
		//generateLineToFile("input/bezierAndLine2.txt",  glm::vec4(960, 244, 0, 1), glm::vec4(640, 632, 0, 1), 0.001f, glm::vec4(1, 1, 0, 1));
		//generateLineToFile("input/bezierAndLine3.txt", glm::vec4(10, 10, 0, 1), glm::vec4(100, 100, 0, 1), 0.001f, glm::vec4(0,1,1,1));

		//monteCarlo.readInputFromFile("input/bezierAndLine1.txt");
		//monteCarlo.readInputFromFile("input/bezierAndLine2.txt");
		//monteCarlo.readInputFromFile("input/bezierAndLine3.txt");

		//generateBezierToFile("input/twoHalves1.txt", { glm::vec4(640, 100, 0, 1),  glm::vec4(700, 244, 0, 1),  glm::vec4(700, 488, 0, 1), glm::vec4(320, 488, 0, 1)}, 0.001f, glm::vec4(0,1,0,0));
		//generateBezierToFile("input/twoHalves2.txt", { glm::vec4(960, 244, 0, 1),  glm::vec4(700, 244, 0, 1),  glm::vec4(700, 488, 0, 1), glm::vec4(640, 632, 0, 1) }, 0.001f, glm::vec4(1, 1, 0, 0));

		monteCarlo.readInputFromFile("input/twoHalves1.txt");
		monteCarlo.readInputFromFile("input/twoHalves2.txt");

		//monteCarlo.readInputFromFile("input/triangle1.txt");
		//monteCarlo.readInputFromFile("input/triangle2.txt");
		//monteCarlo.readInputFromFile("input/triangle3.txt");
		/*monteCarlo.readInputFromFile("input/triangleAndPoint.txt");
		monteCarlo.readInputFromFile("input/triangleAndPoint2.txt");
		monteCarlo.readInputFromFile("input/triangleAndPoint3.txt");
		monteCarlo.readInputFromFile("input/triangleAndPoint4.txt");
		*/monteCarlo.setBVHMode(true);

		monteCarlo.generateBVH();
	}
	catch (const std::invalid_argument& ex) {
		std::cerr << ex.what() << std::endl << "Returning..." << std::endl;
	}

	if (!btnCallBack.coordsRead) {
		std::cout << "HERE";
		monteCarlo.getValueAtMouse(static_cast<int>(btnCallBack.xCoord), static_cast<int>(btnCallBack.yCoord));
	}
	monteCarlo.setDebugMode(false);
	monteCarlo.cpySSBOStoGPU();

	if (firstRun) {
		start = std::chrono::system_clock::now();
		firstRun = false;
	}

	while (!glfwWindowShouldClose(this->window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}
		monteCarlo.draw();

		if (monteCarlo.isDone()) {
			std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
			std::chrono::duration<double> seconds = end - start;
			std::cout << "--------------------------------" << std::endl << "Done: " << seconds.count()  << std::endl;
		}

		configureImGui();

		glfwSwapBuffers(this->window);
	}
}


void Application::generateBezierToFile(std::string fileSrc, const std::vector<glm::vec4>& contP, float gran, const glm::vec4& col) {
	std::vector<glm::vec4> points = HelperFunctions::calculateBezierCurve(contP, gran);

	std::ofstream file(fileSrc);
	file << 4 << " " << 4 << std::endl << std::endl;
	
	glm::vec4 red(1, 0, 0, 1), green(0, 1, 0, 1), blue(0, 0, 1, 1);

	for (const glm::vec4& point : points) {
		file << point.x << ";" << point.y << ";" << point.z << ";" << point.w << ",";
		file << col.x << ";" << col.y << ";" << col.z << ";" << col.w << std::endl;
	}

	file.close();
}

void Application::generateLineToFile(std::string fileSrc, const glm::vec4& startP, const glm::vec4& endP, float gran, const glm::vec4& col) {
	std::vector<glm::vec4> points = HelperFunctions::calculateLine(startP, endP, gran);

	std::ofstream file(fileSrc);
	file << 4 << " " << 4 << std::endl << std::endl;
	size_t it = 0ull;

	for (const glm::vec4& point : points) {
		file << point.x << ";" << point.y << ";" << point.z << ";" << point.w << ",";
		file << col.x << ";" << col.y << ";" << col.z << ";" << col.w << std::endl;
		it++;
	}

	file.close();
}
