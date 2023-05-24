#include <stdio.h>
#include <windows.h>
#include <ctype.h>
#include <stdlib.h>
#include <fstream>
#include <conio.h>
#include <locale.h>
#include <iostream>
using namespace std;
struct mineral
{
	char name[20];
	char category[20];
	char formula[20];
	char symbol[20];
	char classification[20];
};
void addminerals()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	struct mineral t[20];
	char* m = new char[20];
	char p[20];
	unsigned k=1, i;
	ofstream file;
	file.open("minerals.txt", std::ios::app);
	for (i = 0; i < k; ++i)
	{
		printf("¬ведите название минерала:");
		cin >> t[i].name;
		file << t[i].name << " " << endl;
		//scanf_s("%s"&t[i].slovo);
		printf("¬ведите его категорию:");
		cin >> t[i].category;
		file << t[i].category << " " << endl;
		//scanf_s("%s", &t[i].perevod);
		printf("¬ведите его формулы:");
		cin >> t[i].formula;
		file << t[i].formula << " " << endl;
		printf("¬ведите его символ:");
		cin >> t[i].symbol;
		file << t[i].symbol << " " << endl;
		printf("¬ведите его классификацию:");
		cin >> t[i].classification;
		file << t[i].classification << " " << endl;
	}
}
