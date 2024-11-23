//If the code doesnt compile and throws an error about the below code, just comment out the define
#define LAPTOP

#ifdef LAPTOP
#include <Windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

#include "graphics/application.hpp"
#include <stdint.h>


int main() {
	size_t size = sizeof(uint32_t);
	size_t size2 = sizeof(unsigned int);
	std::cout << (size == sizeof(size_t) ? "true" : "false");

	Application application(static_cast<GLint>(1280), static_cast<GLint>(736), "test");
	//application.createContext();
	application.mainLoop();

	return 0;
}