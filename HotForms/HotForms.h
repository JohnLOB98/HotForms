#include <functional>
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
	uint x, y, width, height;
} Rect;

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

		uint y = CoordY;
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
	COORD dwBufferSize, dwBufferCoord;
	SMALL_RECT lpWriteRegion;
};

class Control {
public:

	Control() : rectangle({ 1, 1, 10, 3}), name("button1"), tag(""), fontColor(White), 
				backgroundColor(White) {}
	Control(const string& _name, const Rect& _rectangle) : 
		rectangle(_rectangle), name(_name), tag(""), fontColor(White), backgroundColor(White){}
	~Control() {
		std::cout << "Destructor Base Control";
	}

	//virtual void OnClick(bool* key, void *func) {
	//	std::cout << "OnClick Base Control.";
	//}
	virtual void DrawControl(ConsoleApplication* CA) {
		std::cout << "DrawControl Base Control.";
	}
	
	bool GetKeyUp(int VK, bool* Key) {
		if (GetKeyState(VK) < 0) *Key = true;
		else if (*Key) {
			*Key = false;
			return true;
		}
		return false;
	}

	Rect GetRectangle() { return rectangle; }
	string GetName() { return name; }
	string GetTag() { return tag; }
	byte GetFontColor() { return fontColor; }
	byte GetBackgroundColor() { return backgroundColor; }


	// Passing parameters by reference does not create a copy, like the by value does. If
	// objects of type T are huge -- expensive to copy -- then passing by reference is 
	// more efficient.
	void SetName(const string& value) { name = value; }
	void SetTag(const string& value) { name = value; }
	void SetFontColor(const byte& value) { fontColor = value; }
	void SetBackgroundColor(const byte& value) { backgroundColor = value; }
	void SetRectangle(const Rect& value) { rectangle = value; }

private:

	Rect rectangle;
	string name, tag;
	byte fontColor, backgroundColor;
};




class Button : public Control {
public:
	Button(string _name, Rect _rectangle) : Control(_name, _rectangle), Text(L"hola") {}


	wstring GetText() { return Text; }

	void SetText(const wstring& value) { Text = value; }

	virtual void DrawControl(ConsoleApplication* CA) override {

		Rect r = this->GetRectangle();
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

		CA->DrawStringW(GetRectangle().x, GetRectangle().y, s0, GetFontColor());
		CA->DrawStringW(GetRectangle().x, GetRectangle().y + 1, s1, GetFontColor());
		CA->DrawStringW(GetRectangle().x, GetRectangle().y + 2, s2, GetFontColor());
	}

	// TRIGGERS
	template<typename F>
	void OnClick(bool* key, F function) {

		CONSOLE_SELECTION_INFO PCSI = {};
		COORD Anchor = PCSI.dwSelectionAnchor;
		COORD C = GetConsoleSelectionInfo(&PCSI) ? COORD{ Anchor.X, Anchor.Y } : COORD{ 0,0 };
			
		Rect r = GetRectangle();
		uint btnX = r.x;
		uint btnY = r.y;
		uint btnWidth = r.width;
		uint btnHeight = r.height;

		//bool conditions [] = { C.X >= btnX, C.X <= btnX + btnWidth,
		//					   C.Y >= btnY, C.Y <= btnY + btnHeight}
		bool flag = GetKeyUp(FROM_LEFT_1ST_BUTTON_PRESSED, key);
		if (C.X >= btnX && C.X <= btnX + btnWidth &&
			C.Y >= btnY && C.Y <= btnY + btnHeight &&
			flag) {

			function();
		}
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

	virtual void Initialize(ConsoleApplication* _CA) {
		std::cout << "Initilize Base Window.";
	}
	virtual void Input() {}
	virtual void Update() {}
	virtual void Render() {}

private:
};


/// CUSTOM WINDOWS
class MainWindow : Window {
public:

	MainWindow(ConsoleApplication* _CA) : CA(_CA) {};

	virtual void Initialize(ConsoleApplication* _CA) override {
		std::cout << "Initilize Main Window.";
		CA = _CA;
		*KeyLeftMouse = false;

			
	}
	virtual void Input() override {
		

		//KeyLeftMouse(vKey);	

		// USE LAMBDA EXPRESSION 
		btnTest->OnClick(KeyLeftMouse, [this]() {
			std::cout << R"(Hello World. \n)";
			}
		);


	}
	virtual void Update() override {
		CA->ClearScreen();

		btnTest->DrawControl(CA);
	}
	virtual void Render() override {
		WriteConsoleOutput(CA->GetHout(), 
						   CA->GetBuffer(), 
						   CA->GetdwBufferSize(),
						   CA->GetdwBufferCoord(), 
						   CA->GetlpWriteRegion());
	}


	// TRIGGERS FUNCTION
	//void OnClick_btnTest() {
	//	std::cout << "Hello Button.";
	//}

private:
	ConsoleApplication* CA;
	Button* btnTest = new Button("btnTest", { 1, 1, 10, 3 });
	bool* KeyLeftMouse = new bool;
};

