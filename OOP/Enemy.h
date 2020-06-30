#pragma once
#include "GameObject.h"

class Screen;

class Enemy :
	public GameObject
{

public:
	Enemy(Screen& screen, int pos, const char* shape);

	~Enemy() {}

	// overriding : ������
	void process_input(int key);

	// overriding
	void update();

	void OnDamage(GameObject* collider);
};

class BlinkableEnemy :
	public Enemy
{
	bool isBlinking;
	int count;

public:
	BlinkableEnemy(Screen& screen, int pos, const char* shape)
		: Enemy(screen, pos, shape), isBlinking(false), count(0)
	{}

	void setBlinking() { isBlinking = true; count = 10; }

	// overriding : ������
	void process_input(int key);

	// overriding
	void update();
	void OnDamage(GameObject* collider) { setBlinking(); }

	// overriding
	void draw();
};

