// main.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <ctime>
#include <cstdlib> 
#include "Utils.h"

using namespace std;

// forward declaration

class Screen {
	char* buffer;
	int width;
	int height;

	Screen(int width = 10, int height = 10)
		: width(width), height(height), buffer(new char[getSize()])
	{
		Borland::initialize();
		buffer[getSize() - 1] = '\0';
	}
	static Screen* instance;

public:

	static Screen* getInstance() {
		if (instance == nullptr) {
			instance = new Screen{ 80, 20 };
		}
		return instance;
	}

	~Screen() { delete[] buffer; }

	int getWidth() const { return width; }
	int getScreenWidth() const { return width + 1; }
	int getHeight() const { return height; }
	int getSize() const { return getScreenWidth()*height; }

	void clear() { memset(buffer, ' ', getSize()); buffer[getSize() - 1] = '\0';  }

	void draw(int x, int y, char shape) { buffer[y* getScreenWidth() + x] = shape; }

	void render()
	{
		for (int y = 0; y < height - 1; y++) {
			buffer[y * getScreenWidth() + width] = '\n';
		}
		buffer[getSize()-1] = '\0';

		Borland::gotoxy(0, 0);
		cout << buffer;
	}
};

Screen* Screen::instance = nullptr;

// �̱���
// Ŭ������ ������ �ν��Ͻ��� �ϳ��� �����ϵ��� ����� ��.
// ������ �Լ��� �� �ѹ� �θ��� ��
// �̱��� ����
// 0. ������ �Լ��� private�ϰ� ����
// 1. �ܺο� ����� getInstance��� static �Լ��� ����ϵ��� �����Ѵ�.(������ �ּ� ��ȯ)
// 2. ���� ������ ��ü �ν��Ͻ��� ������ instance ������ private�ϰ� ����

// ����ϴ� ����
// ���� ������� �ʴ´ٸ� �ƿ� �ν��Ͻ��� �������� �ʴ´�.
// ��Ÿ�ӿ� �ʱ�ȭ�ȴ�.
// �̱����� ����� �� �ִ�.


// singleton
class InputManager {

	INPUT_RECORD irInBuf[128];

	HANDLE hStdin;
	DWORD fdwSaveOldMode;

	InputManager() : hStdin(GetStdHandle(STD_INPUT_HANDLE)) {
		if (hStdin == INVALID_HANDLE_VALUE)
			ErrorExit("GetStdHandle");
		if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
			ErrorExit("GetConsoleMode");
		if (!SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			ErrorExit("SetConsoleMode");
	}

	VOID ErrorExit(const char* lpszMessage)
	{
		Borland::gotoxy(0, 21);
		printf("%80d\r", ' ');
		printf("%s\n", lpszMessage);

		// Restore input mode on exit.

		SetConsoleMode(hStdin, fdwSaveOldMode);

		ExitProcess(0);
	}

	VOID KeyEventProc(KEY_EVENT_RECORD ker)
	{
		Borland::gotoxy(0, 22);
		printf("%80d\r", ' ');

		printf("Key event: %c  %d        ", ker.uChar, ker.wRepeatCount);

		if (ker.bKeyDown)
			printf("key pressed\n");
		else 
			printf("key released\n");
		
	}

	VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
	{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
		Borland::gotoxy(0, 22);
		printf("%80d\r", ' ');

		printf("Mouse event: %d %d      ", mer.dwMousePosition.X, mer.dwMousePosition.Y);

		switch (mer.dwEventFlags)
		{
		case 0:

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				printf("left button press \n");
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			{
				printf("right button press \n");
			}
			else
			{
				printf("button press\n");
			}
			break;
		case DOUBLE_CLICK:
			printf("double click\n");
			break;
		case MOUSE_HWHEELED:
			printf("horizontal mouse wheel\n");
			break;
		case MOUSE_MOVED:
			printf("mouse moved\n");
			break;
		case MOUSE_WHEELED:
			printf("vertical mouse wheel\n");
			break;
		default:
			printf("unknown\n");
			break;
		}
	}

	VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
	{
		Borland::gotoxy(0, 22);
		printf("%80d\r", ' ');
		printf("Resize event:             ");
		printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
	}

	static InputManager* instance;
public:
	static InputManager* getInstance() {
		if (instance == nullptr) {
			instance = new InputManager;
		}
		return instance;
	}

	void readInputs() {
		DWORD cNumRead;
		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			ErrorExit("ReadConsoleInput");

		for (int i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			case MOUSE_EVENT: // mouse input 
				MouseEventProc(irInBuf[i].Event.MouseEvent);
				break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
				ResizeEventProc(irInBuf[i].Event.WindowBufferSizeEvent);
				break;

			case FOCUS_EVENT:  // disregard focus events 

			case MENU_EVENT:   // disregard menu events 
				break;

			default:
				ErrorExit("Unknown event type");
				break;
			}
		}
		// debugging
		Borland::gotoxy(0, 21);
		printf("cNumRead = %d", cNumRead);
	}
};

InputManager* InputManager::instance = nullptr;

int main()
{
	Screen* screen = Screen::getInstance();
	InputManager* inputManager = InputManager::getInstance();
	
	bool requestExit = false;
	int x = 0, y = 0;

	while (requestExit == false)
	{
		screen->clear();

		inputManager->readInputs();

		screen->draw(x, y, '0' + x);
		screen->render();

		Sleep(100);

					
	}
	printf("\n���������� ����Ǿ����ϴ�.\n");
	return 0;
}

