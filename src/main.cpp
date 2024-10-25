


//If the code doesnt compile and throws an error about the below code, just comment out the define
#include <Windows.h>

#define LAPTOP

#ifdef LAPTOP
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif



int main() {

	return 0;
}