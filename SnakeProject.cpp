#include "stdafx.h"
#include "GameBoard.h"
#include <windows.h>
#include <iostream>
#include <conio.h>

int Animation()
{
	SnakeGame::GameBoard gameBoard;
	bool exit = false;
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	while (!gameBoard.IsExit() && exit == false)
	{
		gameBoard.MakeMovement();
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			exit = true;
		}

	}
	system("cls");
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	_getch();
	return 0;
}
