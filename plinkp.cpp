#include <iostream>
#include <windows.h>
#include <time.h>
#define SIZE_Y 15
#define SIZE_X 11
#define ITEM (char)254

float mod(float a)
{
	return a > 0 ? a : -a;
}

void clear()
{
  static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);  
  SetConsoleCursorPosition(h,COORD{0, 0});
}

void init()
{
	CONSOLE_CURSOR_INFO cur{1, false};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cur);
	srand(time(0));
	system("cls");
}

class Item
{
	public:
	struct { int x, y; } pos;
	float speed = 0.02f;
	int size = 3;
	Item()
	{}
	
	Item(int x, int y, int s = 3) : size(s), pos{x, y}
	{}
	
	Item operator= (const Item& it)
	{
		pos.x = it.pos.x; pos.y = it.pos.y;
		speed = it.speed;
		size = it.size;
	}
	
	Item(const Item& it)
	{
		*this = it;
	}
};

class Grid
{
	char grid[SIZE_Y][SIZE_X];
	public:
	Grid()
	{
		for (int i = 0; i < SIZE_Y; i++)
			for (int i1 = 0; i1 < SIZE_X; i1++)
			{
				if (i == SIZE_Y - 1)
					grid[i][i1] = '#';
				else
				if (i1 == 0 || i1 == SIZE_X - 1)
					grid[i][i1] = '#';
				else
					grid[i][i1] = ' ';
			}
	}
	
	Item add_item(const Item& it)
	{
		Item prom = it;
		int change = it.size;
		for (int i = 0; i < it.size; i++)
			if (it.pos.y == 13 || grid[it.pos.y + 1][it.pos.x + i] == ITEM)
				grid[it.pos.y][it.pos.x + i] = ITEM;
			else
				prom.size--;
		
		prom.pos.x = (rand() % 2)? 1 : SIZE_X - prom.size - 1;
		prom.speed *= 1.2f; prom.speed = prom.pos.x == 1? prom.speed : -mod(prom.speed);
		prom.pos.y -= 1;
		
		return prom;
	}
	
	void print()
	{
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int i1 = 0; i1 < SIZE_X; i1++)
				std::cout << grid[i][i1];
			std::cout << '\n';
		}
	}
	
	void print(const Item& it)
	{
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int i1 = 0; i1 < SIZE_X; i1++)
				if (i1 >= it.pos.x && i1 < it.pos.x + it.size && i == it.pos.y)
					std::cout << ITEM;
				else
					std::cout << grid[i][i1];
				std::cout << '\n';
		}
	}	
};

class Space
{
	short int lastbutton, curbutton;
	public:
		bool check()
		{
			curbutton = GetAsyncKeyState(VK_SPACE);
			if (curbutton == lastbutton || curbutton + 1 == lastbutton)
				return false;
			lastbutton = curbutton;
			return lastbutton & 0x8001;
		}
};

int main()
{
	init();
		
	Grid grid;
	Item curitem(1, 13);
	Space button;
	
	while (curitem.size && curitem.pos.y >= 0)
	{
		float offsetX = 0.0f;
		
		if (curitem.pos.y == 9 && curitem.size > 2)	
			curitem.size = 2;

		if (curitem.pos.y == 6 && curitem.size > 1)
			curitem.size = 1;
		
		while (!button.check())
		{
			offsetX += curitem.speed;
			curitem.pos.x += (int)offsetX;
			offsetX = mod(offsetX) > 1 ? 0.0f : offsetX;
			
			if ((curitem.pos.x + curitem.size) == SIZE_X - 1 && curitem.speed > 0 || curitem.pos.x == 1 && curitem.speed < 0)
				curitem.speed *= -1;
				
			grid.print(curitem);
			clear();
		}
		curitem = grid.add_item(curitem);
		grid.print();
		clear();            
	}
	
	for (int i = 0; i < 3; i++)
	{
		system("cls"); Sleep(600);
		grid.print();  Sleep(600);
	}
	
	system("cls");
	
	if (curitem.size > 0)
	{
		std::cout << "You won";
		return 0;
	}
	
	std::cout << "Try again?\n Y / N";
	while (!GetAsyncKeyState('Y') || !GetAsyncKeyState('N'))
	{
		if (GetAsyncKeyState('Y'))
			main();
			
		if (GetAsyncKeyState('N'))
		{
			system("cls");
			std::cout << "You lost";
			return 0;
		}
	}
	
}
