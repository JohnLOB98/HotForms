#include "HotForms.h"


int Execution(MainWindow Wnd) {

		while (1) {

		Wnd.Input();
		Wnd.Update();
		Wnd.Render();
		
	}

}

//int Execution(Window Wnd) {
//
//	//Wnd.Initialize(CA);
//
//	while (1) {
//
//		Wnd:Input();
//		Wnd.Update();
//		Wnd.Render();
//		
//	}
//
//}

int main() {
	
	static const uint bufferWidth = 81;
	static const uint bufferHeight = 23;

	CHAR_INFO* buffer;
	HWND hwnd;
	HANDLE hout;
	COORD dwBufferSize = { bufferWidth, bufferHeight };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT lpWriteRegion = { 0, 0, bufferWidth - 1, bufferHeight - 1 };

	hout = GetStdHandle(STD_OUTPUT_HANDLE);
	int length = bufferWidth * bufferHeight;
	buffer = (CHAR_INFO*)malloc(length * sizeof(*buffer));
	hwnd = { GetConsoleWindow() };


	ConsoleApplication* CA = new ConsoleApplication(buffer, hwnd, hout, dwBufferSize, dwBufferCoord, lpWriteRegion);

	MainWindow mainWnd = MainWindow(CA);

	Execution(mainWnd);

	//while (1) {

	//	Sleep(1);
	//}

	return 0;
}