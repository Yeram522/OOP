// main.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib> // include malloc,

#include "Screen.h"
#include "Enemy.h"
#include "AlphabetEnemy.h"
#include "Player.h"
#include "Bullet.h"
#include "UI.h"


int main()
{
	Screen screen{ 80 };

	new Player{ screen, "(o_o)" };	
	UI* uiTotal = new UI{ screen, "t : ", 0, 2 + 1 };
	UI* uiBullets = new UI{ screen, "b: ", uiTotal->getEndpoint(), 2 + 1 };
	UI* uiActiveBullets = new UI{ screen, "ab: ", uiBullets->getEndpoint(), 2 + 1 };
	UI* uiEnemies = new UI{ screen, "e: ", uiActiveBullets->getEndpoint(), 2 + 1 };

	int startOffset = uiEnemies->getEndpoint();

	int nFramesToSpawnEnemy = 30;
	
	bool requestExit = false;
	while (requestExit == false)		
	{
		if (--nFramesToSpawnEnemy == 0) {
			new AlphabetEnemy{ screen };
			nFramesToSpawnEnemy = 30;
		}

		screen.clear();

		GameObject** gos = GameObject::getGameObjects();
		int capacity = GameObject::getMaxGameObjects();

		// update game objects (player, enemy ...)
		// gos, maxGameObjects
		for (int i = 0; i < capacity; i++)
		{	
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;			

			// obj != nullptr
			// search player
			Player* player = dynamic_cast<Player *>(obj); // dynamically downcast			
			if (player != nullptr) {
				// if player exists, check whether it is inside screen. otherwise, exit.

				if (player->isInside() == false) {
					requestExit = true;
					break;
				}
				continue;
			}
		}

		if (_kbhit()) {
			int key = _getch();
			if (key == 'z') {
				break; // exit from main loop
			}
			for (int i = 0; i < capacity; i++)
			{
				if (gos[i] == nullptr) continue;
				gos[i]->process_input(key);
			}
		}
		int nEnemies = 0;
		int nPlayers = 0;
		int nBullets = 0;
		int nActiveBullets = 0;
		for (int i = 0; i < capacity; ++i)
		{
			if (gos[i] == nullptr) continue;
			if (dynamic_cast<Enemy*>(gos[i])) ++nEnemies;
			if (dynamic_cast<Player*>(gos[i])) ++nPlayers;
			if (dynamic_cast<Bullet*>(gos[i])) {
				++nBullets;
				if (static_cast<Bullet*>(gos[i])->checkFire() == true) {
					++nActiveBullets;
				}
			}
		}
		uiTotal->setData(capacity);
		uiBullets->setData(nBullets);
		uiActiveBullets->setData(nActiveBullets);
		uiEnemies->setData(nEnemies);


		gos = GameObject::getGameObjects();
		capacity = GameObject::getMaxGameObjects();
		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == nullptr) continue;
			gos[i]->update();
		}


		for (int i = 0; i < capacity; i++)
		{
			if (gos[i] == nullptr) continue;
			gos[i]->draw();
		}

		gos = GameObject::getGameObjects();
		capacity = GameObject::getMaxGameObjects();
		for (int i = 0; i < capacity; i++)
		{
			GameObject* obj = gos[i];
			if (obj == nullptr) continue;
			if (obj->isActive() == false) {
				delete obj;
			}
		}
		
		// display canvas to a monitor
		screen.render();
		Sleep(100);
	}
	printf("\n���������� ����Ǿ����ϴ�.\n");
	return 0;
}
