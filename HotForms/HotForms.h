#include <iostream>
#include <windows.h>
#include <string>

typedef std::string string;
typedef std::wstring wstring;
typedef unsigned int uint;
typedef unsigned char ubyte;

enum Colors {
	White = 0x0F,
	Black = 0x00
};

typedef struct _Rectangle {
	//public	
	uint x, y, width, height;
	//_Rectangle(uint _x, uint _y, uint _width, uint _height) : x(_x), y(_y), width(_width), height(_height) {}

	//~_Rectangle() {
	//	std::cout << "The rectangle has been destroyed.";
	//}


	//uint GetX() { return x; }
	//uint GetY() { return y; }
	//uint GetWidth() { return width; }
	//uint GetHeight() { return height; }

	//void SetX(const uint& value) { x = value; }
	//void SetY(const uint& value) { y = value; }
	//void SetWidth(const uint& value) { width = value; }
	//void SetHeight(const uint& value) { height = value; }



//private:
	//uint x, y, width, height;
} Rectangle;

class ConsoleApplication {
public:

	ConsoleApplication(CHAR_INFO* _buffer, const HWND& _hwnd, const HANDLE& _hout,
		const COORD& _dwBufferSize, const COORD& _dwBufferCoord,
		const SMALL_RECT& _lpWriteRegion)
		: buffer(_buffer),
		hwnd(_hwnd), hout(_hout),
		dwBufferSize(_dwBufferSize),
		dwBufferCoord(_dwBufferCoord),
		lpWriteRegion(_lpWriteRegion)
	{

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
		SetCurrentConsoleFontEx(hout, false, &fontex); // Termina de ajustar el tamaño de los caracteres
		// Poner la consola en el centro de la pantalla y ajusta su tamaño
		int X = GetSystemMetrics(SM_CXSCREEN) / 2 - 1492 / 2;
		int Y = GetSystemMetrics(SM_CYSCREEN) / 2 - 800 / 2;
		SetWindowPos(GetConsoleWindow(), NULL, X, Y, 1492, 800, SWP_SHOWWINDOW);

		CONSOLE_CURSOR_INFO info;
		GetConsoleCursorInfo(hout, &info); // dwSize, bVisible
		info.dwSize = 100;
		info.bVisible = FALSE;
		SetConsoleCursorInfo(hout, &info); // Termina de esconder el cursor
		ShowScrollBar(hwnd, SB_VERT, 0); // Remove the scrollbar
		SetConsoleTitle(L"HotForms");

		this->ClearScreen();
	}
	//SetName(_name);

	CHAR_INFO* GetBuffer() { return buffer; }
	HWND GetHwnd() { return hwnd; }
	HANDLE GetHout() { return hout; }
	COORD GetdwBufferSize() { return dwBufferSize; }
	COORD GetdwBufferCoord() { return dwBufferCoord; }
	SMALL_RECT* GetlpWriteRegion() { return &lpWriteRegion; }

	void ClearScreen() {

		int width = this->dwBufferSize.X;
		int height = this->dwBufferSize.Y;

		for (int i = 0; i < width * height; i++) {
			buffer[i].Attributes = White;
			buffer[i].Char.UnicodeChar = L'.';
		}
	}

	void DrawCharW(int CoordX, int CoordY, wchar_t c, ubyte Color) {

		uint y = dwBufferSize.Y - CoordY - 1;
		uint i = y * dwBufferSize.X + CoordX;

		buffer[i].Char.UnicodeChar = c;
		buffer[i].Attributes = Color;
	}
	void DrawStringW(int CoordX, int CoordY, wstring s, ubyte Color) {

		for (int i = 0; i < s.length(); i++) {
			DrawCharW(CoordX + i, CoordY, s[i], Color);
		}
	}

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

class Control {
public:

	Control() {}
	Control(const string& _name, const Rectangle& _rectangle) : name(_name), rectangle(_rectangle) {}

	~Control() {
		std::cout << "This control was destroyed";
	}

	virtual void DrawControl(ConsoleApplication* CA) {
		std::cout << "Draw control base.";
	}

	Rectangle GetRectangle() { return rectangle; }
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
	void SetRectangle(const Rectangle& value) { rectangle = value; }

private:

	Rectangle rectangle;
	string name, tag;
	byte fontColor, backgroundColor;
};




class Button : public Control {
public:
	Button(string _name, Rectangle _rectangle) : Control(_name, _rectangle), Text(L"hola") {}


	wstring GetText() { return Text; }

	void SetText(const wstring& value) { Text = value; }

	virtual void DrawControl(ConsoleApplication* CA) override {

		Rectangle r = this->GetRectangle();
		int x = r.x;
		int y = r.y;
		int Width = r.width;
		ubyte FontColor = this->GetFontColor();

		wstring s1 = L"│ " + this->Text;
		int l = s1.length();
		for (int i = 0; i <= Width - 2 - l; ++i) s1 += L" ";
		s1 += L"│";

		wstring s0 = L"┌";
		for (int i = 0; i < Width - 2; ++i) s0 += L'─';
		s0 += L"┐";

		wstring s2 = L"└";
		for (int i = 0; i < Width - 2; ++i) s2 += L'─';
		s2 += L"┘";

		CA->DrawStringW(this->GetRectangle().x, this->GetRectangle().y, s0, this->GetFontColor());
		CA->DrawStringW(this->GetRectangle().x, this->GetRectangle().y, s1, this->GetFontColor());
		CA->DrawStringW(this->GetRectangle().x, this->GetRectangle().y, s2, this->GetFontColor());
	}

private:
	wstring Text;
};

class InputField : private Control {
public:
private:

};

class Text : private Control {
public:
private:

};

class Window : Control {
public:

	virtual void Initialize() {
		std::cout << "Initilize Base Window.";
	}
	virtual void Input() {}
	virtual void Update() {}
	virtual void Render(ConsoleApplication* CA) {}

private:
};


/// CUSTOM WINDOWS
class MainWindow : Window {
public:

	virtual void Initialize() override {
		std::cout << "Initilize Main Window.";
	}
	virtual void Input() override {}
	virtual void Update() override {
		btnTest->DrawControl(CA);
	}
	virtual void Render(ConsoleApplication* CA) override {}
private:
	ConsoleApplication* CA;
	Button* btnTest = new Button("btnTest", { 1, 1, 10, 3 });
};