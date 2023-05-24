#include <string>
#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
#include <conio.h>
#include <locale.h>
#include <iostream>
#include<iomanip>
using namespace std;
struct mineral
{
	char name[50];
	char category[50];
	char formula[50];
	char symbol[50];
	char classification[50];
};

void searchname()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[50];
	string* m = new string[50];
	char p[50];
	unsigned count=0, i;
	std::string line;
	std::ifstream in("minerals.txt");
	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	ifstream file;
	file.open("minerals.txt");
	for (i = 0; i < count; ++i)
	{
		file >> t[i].name;
		file >> t[i].category;
		file >> t[i].formula;
		file >> t[i].symbol;
		file >> t[i].classification;
	}
	if (count > 0) 
	{
		printf("введите название для поиска:");
		cin >> p;
		//scanf_s("%s", &p);
		printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		for (i = 0; i < count; ++i)
		{
			if (strcmp(t[i].name, p) == 0)
			{
				cout << "| " << left << setw(15);
				cout << p;
				cout << " | " << left << setw(25);
				cout << t[i].category;
				cout << " | " << left << setw(25);
				cout << t[i].formula;
				cout << " | " << left << setw(10);
				cout << t[i].symbol;
				cout << " | " << left << setw(25);
				cout << t[i].classification;
				cout << " |" << "\n";
			}
		}
	}
}
void searchcategory()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[50];
	string* m = new string[50];
	char p[50];
	unsigned count = 0, i;
	std::string line;
	std::ifstream in("minerals.txt");
	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	ifstream file;
	file.open("minerals.txt");
	for (i = 0; i < count; ++i)
	{
		file >> t[i].name;
		file >> t[i].category;
		file >> t[i].formula;
		file >> t[i].symbol;
		file >> t[i].classification;
	}
	if (count > 0)
	{
		printf("введите категорию для поиска:");
		cin >> p;
		//scanf_s("%s", &p);
		printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		for (i = 0; i < count; ++i)
		{
			if (strcmp(t[i].category, p) == 0)
			{
				cout << "| " << left << setw(15);
				cout << t[i].name;
				cout << " | " << left << setw(25);
				cout << p;
				cout << " | " << left << setw(25);
				cout << t[i].formula;
				cout << " | " << left << setw(10);
				cout << t[i].symbol;
				cout << " | " << left << setw(25);
				cout << t[i].classification;
				cout << " |" << "\n";
			}
		}
	}
}
void searchformula()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[50];
	string* m = new string[50];
	char p[50];
	unsigned count = 0, i;
	std::string line;
	std::ifstream in("minerals.txt");
	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	ifstream file;
	file.open("minerals.txt");
	for (i = 0; i < count; ++i)
	{
		file >> t[i].name;
		file >> t[i].category;
		file >> t[i].formula;
		file >> t[i].symbol;
		file >> t[i].classification;
	}
	if (count > 0)
	{
		printf("введите формулу для поиска:");
		cin >> p;
		//scanf_s("%s", &p);
		printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		for (i = 0; i < count; ++i)
		{
			if (strcmp(t[i].formula, p) == 0)
			{
				cout << "| " << left << setw(15);
				cout << t[i].name;
				cout << " | " << left << setw(25);
				cout << t[i].category;
				cout << " | " << left << setw(25);
				cout << p;
				cout << " | " << left << setw(10);
				cout << t[i].symbol;
				cout << " | " << left << setw(25);
				cout << t[i].classification;
				cout << " |" << "\n";
			}
		}
	}
}
void searchsymbol()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[50];
	string* m = new string[50];
	char p[50];
	unsigned count = 0, i;
	std::string line;
	std::ifstream in("minerals.txt");
	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	ifstream file;
	file.open("minerals.txt");
	for (i = 0; i < count; ++i)
	{
		file >> t[i].name;
		file >> t[i].category;
		file >> t[i].formula;
		file >> t[i].symbol;
		file >> t[i].classification;
	}
	if (count > 0)
	{
		printf("введите символ для поиска:");
		cin >> p;
		//scanf_s("%s", &p);
		printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		for (i = 0; i < count; ++i)
		{
			if (strcmp(t[i].symbol, p) == 0)
			{
				cout << "| " << left << setw(15);
				cout << t[i].name;
				cout << " | " << left << setw(25);
				cout << t[i].category;
				cout << " | " << left << setw(25);
				cout << t[i].formula;
				cout << " | " << left << setw(10);
				cout << p;
				cout << " | " << left << setw(25);
				cout << t[i].classification;
				cout << " |" << "\n";
			}
		}
	}
}
void searchclassification()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[50];
	string* m = new string[50];
	char p[50];
	unsigned count = 0, i;
	std::string line;
	std::ifstream in("minerals.txt");
	if (in.is_open())
	{
		while (std::getline(in, line))
		{
			count++;
		}
	}
	ifstream file;
	file.open("minerals.txt");
	for (i = 0; i < count; ++i)
	{
		file >> t[i].name;
		file >> t[i].category;
		file >> t[i].formula;
		file >> t[i].symbol;
		file >> t[i].classification;
	}
	if (count > 0)
	{
		printf("введите классифткацию для поиска:");
		cin >> p;
		//scanf_s("%s", &p);
		printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
		printf("--------------------------------------------------------------------------------------------------------------------\n");
		for (i = 0; i < count; ++i)
		{
			if (strcmp(t[i].classification, p) == 0)
			{
				cout << "| " << left << setw(15);
				cout << t[i].name;
				cout << " | " << left << setw(25);
				cout << t[i].category;
				cout << " | " << left << setw(25);
				cout << t[i].formula;
				cout << " | " << left << setw(10);
				cout << t[i].symbol;
				cout << " | " << left << setw(25);
				cout << p;
				cout << " |" << "\n";
			}
		}
	}
}
