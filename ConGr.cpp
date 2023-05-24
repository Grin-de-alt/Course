#include <windows.h> 
#include <iostream>
#include "Menu.h" 
#include "Examples.h"
#include "Keyboard.h"

int Minerals();
void aboutMe();
void Galton(COLORREF color);
int Animation();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	std::string menuItems[] = { "Графическая заставка","Доска Гальтона","Справочник","Об авторе","Выход"};
	for (;;) { 
		int choise = ShowMenu(menuItems, 5); 
		system("cls");
		switch (choise)
		{
		default:
			return 0;
		case 0:
			Animation();
			GetKey();
			break;
		case 1:
			Galton(RGB(225,225,225));
			GetKey();
			break;
		case 2:
			Minerals();
			GetKey();
			break;
		case 3:
			aboutMe();
			GetKey();
			break;
		}
		system("cls");
	}
}
