#include "graphics/application.hpp"


//If the code doesnt compile and throws an error about the below code, just comment out the define
#define LAPTOP

#ifdef LAPTOP
#include <Windows.h>
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



int main() {

	Application application(static_cast<GLint>(1280), static_cast<GLint>(720), "test");
	application.createContext();
	application.mainLoop();

	return 0;
}