#include <iostream>
#include <string>
#include <fstream>
#include <locale.h>
#include <Windows.h>
#include <windows.h> 
#include <iostream>
#include "Menu.h" 
#include "Examples.h"
#include "Keyboard.h"
using namespace std;

int Minerals()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	std::string menuItems[] = {"Показать список Минералов","Поиск минерала","Добавить","Выход" };
	for (;;) {
		int choise = ShowMenu(menuItems, 4);
		system("cls");
		switch (choise)
		{
		default:
			return 0;
		case 0:
			listminerals();
			GetKey();
			break;
		case 1:
			searchminerals();
			GetKey();
			break;
		case 2:
			addminerals();
			GetKey();
			break;
		}
		system("cls");
	}
}