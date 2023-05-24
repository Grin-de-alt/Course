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
void searchname();
void searchcategory();
void searchformula();
void searchsymbol();
void searchclassification();
int searchminerals()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "rus");
	std::string menuItems[] = { "По названию","По категории","По формуле","По символу","По классификации","Выход" };
	for (;;) {
		int choise = ShowMenu(menuItems, 6);
		system("cls");
		switch (choise)
		{
		default:
			return 0;
		case 0:
			searchname();
			GetKey();
			break;
		case 1:
			searchcategory();
			GetKey();
			break;
		case 2:
			searchformula();
			GetKey();
			break;
		case 3:
			searchsymbol();
			GetKey();
			break;
		case 4:
			searchclassification();
			GetKey();
			break;
	}
		system("cls");
	}
}