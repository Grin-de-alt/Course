#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include<iomanip>
using namespace std;

struct mineral
{
    std::string name;
    std::string category;
    std::string formula;
    std::string symbol;
    std::string classification;
};

void listminerals()
{
    std::ifstream inputFile("minerals.txt");

    assert(inputFile.is_open());

    mineral tmp;
    printf("|      NAME       |         CATEGORY          |         FORMULA           |   SYMBOL   |       CLASSIFICATION      |\n");
    printf("--------------------------------------------------------------------------------------------------------------------\n");
    while (inputFile >> tmp.name >> tmp.category >> tmp.formula >> tmp.symbol >> tmp.classification) {
        cout << "| " << left << setw(15);
        cout << tmp.name;
        cout << " | " << left << setw(25);
        cout << tmp.category;
        cout << " | " << left << setw(25);
        cout << tmp.formula;
        cout << " | " << left << setw(10);
        cout << tmp.symbol;
        cout << " | " << left << setw(25);
        cout << tmp.classification;
        cout << " |" << "\n";
    }
}