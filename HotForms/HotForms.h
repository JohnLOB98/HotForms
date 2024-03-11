#pragma once
#include <iostream>
#include <windows.h>
#include <string>

typedef std::string string;
typedef unsigned int uint;
typedef unsigned char byte;

enum Colors {
	White = 0x0F,
	Black = 0x00
};

class Rectangle {
public:

	Rectangle(uint _x, uint _y, uint _width, uint _height) : x(_x), y(_y), width(_width), height(_height) {}

	~Rectangle() {
		std::cout << "The rectangle has been destroyed.";
	}


	uint GetX() { return x; }
	uint GetY() { return y; }
	uint GetWidth() { return width; }
	uint GetHeight() { return height; }

	void SetX(const uint& value) { x = value; }
	void SetY(const uint& value) { y = value; }
	void SetWidth(const uint& value) { width = value; }
	void SetHeight(const uint& value) { height = value; }

	

private:
	uint x, y, width, height;
};


class Control {
public:

	string GetName() { return name; }
	string GetTag() { return tag; }

	byte GetFontColor() { return fontColor; }
	byte GetBackgroundColor() { return backgroundColor; }
	
	//Passing parameters by reference does not create a copy, like the by value does.If objects of type T are huge 
	//-- expensive to copy -- then passing by reference is more efficient.
	void SetName(const string& value) { name = value; }
	void SetTag(const string& value) { name = value; }
	void SetFontColor(const byte& value) { fontColor = value; }
	void SetBackgroundColor(const byte& value) { backgroundColor = value; }

private:
	string name, tag;
	byte fontColor, backgroundColor;
};

class ConsoleApplication {
public:

	//void SetWidth(uint value) { bufferWidth = value; }
	//void SetHeigth(uint value) { bufferHeight = value; }

	ConsoleApplication(CHAR_INFO* _buffer, const HWND& _hwnd, const HANDLE& _hout, 
					   const COORD& _dwBufferSize, const COORD& _dwBufferCoord, const SMALL_RECT& _lpWriteRegion)
					   : buffer(_buffer), hwnd(_hwnd), hout(_hout), dwBufferSize(_dwBufferSize), dwBufferCoord(_dwBufferCoord),
						 lpWriteRegion(_lpWriteRegion){
			
		SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
		static CONSOLE_FONT_INFOEX fontex; // Comienza a ajustar el tamaño de los carcteres de la consola
		GetCurrentConsoleFontEx(hout, 0, &fontex);
		fontex.cbSize = sizeof(fontex);
		fontex.nFont = 0;
		fontex.dwFontSize.X = 17;
		fontex.dwFontSize.Y = 33;
		fontex.FontWeight = FW_NORMAL;
		fontex.FontFamily = FF_DONTCARE;
		lstrcpyW(fontex.FaceName, L"Terminal");
		//fontex.FaceName = L"Lucida Console";
		SetCurrentConsoleFontEx(hout, false, &fontex); // Termina de ajustar el tamaño de los caracteres
		// Poner la consola en el centro de la pantalla y ajusta su tamaño
		SetWindowPos(GetConsoleWindow(), NULL, GetSystemMetrics(SM_CXSCREEN) / 2 - 1492 / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - 800 / 2, 1492, 800, SWP_SHOWWINDOW);

		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(hout, &info); // dwSize, bVisible
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(hout, &info); // Termina de esconder el cursor
		ShowScrollBar(hwnd, SB_VERT, 0); // Remove the scrollbar
		SetConsoleTitle(L"HotForms");
	}
		//SetName(_name);

	CHAR_INFO* GetBuffer() { return buffer; }
/// <summary>
/// FALTA AGREGAR LOS SETTERS Y GETTERS
/// </summary>

private:
	CHAR_INFO* buffer;
	HWND hwnd;
	HANDLE hout;
	COORD dwBufferSize;
	COORD dwBufferCoord;
	SMALL_RECT lpWriteRegion;
	// 
	//HWND hwnd;
	//HANDLE hout;
	//COORD dwBufferSize = { bufferWidth, bufferHeight };
	//COORD dwBufferCoord = { 0, 0 };
	//SMALL_RECT lpWriteRegion = { 0, 0, bufferWidth - 1, bufferHeight - 1 };
};


class Button : private Control {
public:
	Button() {
	}

	

	//Button(x)

private:

};

class InputField : private Control {
public:
private:

};

class Text : private Control {
public:
private:

};