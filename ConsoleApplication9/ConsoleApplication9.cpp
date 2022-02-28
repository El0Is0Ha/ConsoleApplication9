#include <iostream>
#include <random>
#include <stdlib.h>
#include <chrono>
using namespace std;

enum TCell : char
{
	CROSS = 'X',
	ZERO = '0',
	EMPTY = '_'
};

enum TProgress
{
	IN_PROGRESS,
	WON_AI,
	WON_HUMAN,
	DRAW
};

struct TCoord
{
	size_t y{ 0 };
	size_t x{ 0 };
};

struct TGame
{
	const size_t SIZE{ 3 };
	TCell** ppFiled{ nullptr };
	TCell ai;
	TCell human;
	TProgress progress{ IN_PROGRESS };
	size_t turn{ 0 };
};

inline void clearScr()
{
	system("cls");
}

int32_t __fastcall getRandomNum(int32_t min, int32_t max)
{
	const static auto seed = chrono::system_clock::now().time_since_epoch().count();
	static mt19937_64 generator(seed);
	uniform_int_distribution<int32_t> dis(min, max);
	return dis(generator);
}

void __fastcall initGame(TGame & g)
{
	g.ppFiled = new TCell * [g.SIZE];
	for (size_t i = 0; i < g.SIZE; i++)
	{
		g.ppFiled[i] = new TCell[g.SIZE];
	}
	for (size_t y = 0; y < g.SIZE; y++)
	{
		for (size_t x = 0; x < g.SIZE; x++)
		{
			g.ppFiled[y][x] = EMPTY;
		}

	}
	if (getRandomNum(0, 1000) > 500)
	{
		g.human = CROSS;
		g.ai = ZERO;
		g.turn = 0;
	}
	else
	{
		g.human = ZERO;
		g.ai = CROSS;
		g.turn = 1;
	}
}

void __fastcall deinitGame(TGame & g)
{
	for (size_t i = 0; i < g.SIZE; i++)
	{
	delete [] g.ppFiled[i];
	}
	delete[] g.ppFiled;
	g.ppFiled = nullptr;

}

void __fastcall printGame(const TGame & g)
{
	cout << "      ";
	for (size_t x = 0; x < g.SIZE; x++)
	{
		cout << x + 1 << "   ";
	}

	cout << endl;

	for (size_t y = 0; y < g.SIZE; y++)
	{
		cout << " " << y + 1 << " | ";
		for (size_t x = 0; x < g.SIZE; x++)
		{
			cout << g.ppFiled[y][x] << " | ";
		}
		cout << endl;

	}
	cout << endl << "Human: " << g.human << endl << "Computer: " << g.ai << endl;
}

TProgress __fastcall gotWon(TGame & g)
{
	for (size_t y = 0; y < g.SIZE; y++)
	{
		if (g.ppFiled[y][0] == g.ppFiled[y][1] && g.ppFiled[y][0] == g.ppFiled[y][2])
		{
			if (g.ppFiled[y][0] == g.human)
				return WON_HUMAN;
			if (g.ppFiled[y][0] == g.ai)
				return WON_AI;
		}
	}		
	for (size_t x = 0; x < g.SIZE; x++)
	{
		if (g.ppFiled[0][x] == g.ppFiled[1][x] && g.ppFiled[0][x] == g.ppFiled[2][x])
		{
			if (g.ppFiled[0][x] == g.human)
				return WON_HUMAN;

			if (g.ppFiled[0][x] == g.ai)
				return WON_AI;
		}
	}

	//diagonal
	if (g.ppFiled[0][0] == g.ppFiled[1][1] && g.ppFiled[0][0] == g.ppFiled[2][2])
	{
		if (g.ppFiled[0][0] == g.human)
			return WON_HUMAN;

		if (g.ppFiled[0][0] == g.ai)
			return WON_AI;
	}

	if (g.ppFiled[2][0] == g.ppFiled[1][1] && g.ppFiled[1][1] == g.ppFiled[0][2])
	{
		if (g.ppFiled[1][1] == g.human)
			return WON_HUMAN;

		if (g.ppFiled[1][1] == g.ai)
			return WON_AI;
	}
	//draw
	bool draw(true);
	for (size_t y = 0; y < g.SIZE; y++)
	{
	    for (size_t x = 0; x < g.SIZE; x++)
		{
			if (g.ppFiled[y][x] == EMPTY)
			{
				draw = false;;
				break;
			}

		}
		if (!draw)
			break;
	}
	if (draw)
		return DRAW;

	return IN_PROGRESS;

			
}

TCoord __fastcall getHumanCoord(const TGame & g)
{
	TCoord c{ 0 };
	do
	{
		cout << "Enter Y (1...3): ";
		cin >> c.y;
		cout << "Enter X (1...3): ";
		cin >> c.x;

		c.y--;
		c.x--;

	} while (c.y > 2 || c.x > 2 || g.ppFiled[c.y][c.x] != EMPTY);

	return c;
}

TCoord __fastcall getComputerCoord(TGame & g)
{
	//centr
	if (g.ppFiled[1][1] == EMPTY)
	{
		return { 1, 1 };
	}

	//ugl
	TCoord buf[4];
	size_t num = 0;
	
	if (g.ppFiled[0][0] == EMPTY)
	{
		buf[num] = { 0, 0 };
		num++;
	}

	if (g.ppFiled[2][2] == EMPTY)
	{
		buf[num] = { 2, 2 };
		num++;
	}

	if (g.ppFiled[2][0] == EMPTY)
	{
		buf[num] = { 2, 0 };
		num++;
	}

	if (g.ppFiled[0][2] == EMPTY)
	{
		buf[num] = { 0, 2 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}

	//boka
	num = 0;
	if (g.ppFiled[0][1] == EMPTY)
	{
		buf[num] = { 0, 1 };
		num++;
	}

	if (g.ppFiled[2][1] == EMPTY)
	{
		buf[num] = { 2, 1 };
		num++;
	}

	if (g.ppFiled[1][0] == EMPTY)
	{
		buf[num] = { 1, 0 };
		num++;
	}

	if (g.ppFiled[1][2] == EMPTY)
	{
		buf[num] = { 1, 2 };
		num++;
	}
	if (num > 0)
	{
		const size_t index = getRandomNum(0, 1000) % num;
		return buf[index];
	}
}

void congrats(const TGame& g)
{
	if (g.progress == WON_HUMAN)
	{
		cout << "Human WON" << endl;
	}
	else if (g.progress == WON_AI)
	{
		cout << "Computer WON" << endl;
	}
	else if (g.progress == DRAW)
	{
		cout << "DRAW" << endl;
	}
}


//==============================================================

	int main()
{
		TGame g;

		initGame(g);

		clearScr();

		printGame(g);

		do
		{
			if (g.turn % 2 == 0)
			{
				//Human
				TCoord c = getHumanCoord(g);
				g.ppFiled[c.y][c.x] = g.human;
			}
			else
			{
				//ai
				TCoord c = getComputerCoord(g);
				g.ppFiled[c.y][c.x] = g.ai;
			}

			g.turn++;
			clearScr();
            printGame(g);
			g.progress = gotWon(g);


		} while (g.progress == IN_PROGRESS);

		deinitGame(g);

	return 0;
}
