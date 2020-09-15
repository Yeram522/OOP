// main.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
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
			instance = new Screen{ 10,10 };
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


int main()
{
	Screen* screen = Screen::getInstance();
	

	bool requestExit = false;
	int x = 0, y = 0;

	while (requestExit == false)
	{
		screen->clear();
		screen->draw(x, y, '0' + x);
		screen->render();

		// debugging
		Borland::gotoxy(20, 20);
		printf("x = %d, y = %d", x, y);

		Sleep(100);
		
		++x %= screen->getWidth();
		++y %= screen->getHeight();
	}
	printf("\n���������� ����Ǿ����ϴ�.\n");
	return 0;
}
