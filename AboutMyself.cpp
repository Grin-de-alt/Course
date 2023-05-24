#include <Windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <math.h>

std::string name1 = "Выполнил:Курманов Павел\n";
std::string name2 = "Группа:ИВТ-223";
std::vector<int> leters;

void swap(int& a, int& b) 
{
    int buf = a;
    a = b;
    b = buf;
}

void initLeters() {
	for (int i = 0; i < name1.size(); ++i)
    {
        leters.push_back(i);
	}
    while ((rand() % 10) != 7) {
        swap(leters[abs(rand()) % leters.size()], leters[abs(rand()) % leters.size()]);
    }
}

HFONT getFont() {
    LOGFONT font;
    font.lfHeight = 40;
    font.lfWidth = 0;
    font.lfEscapement = 0;
    font.lfOrientation = 0;
    font.lfWeight = 10;
    font.lfItalic = 0;
    font.lfUnderline = 0;
    font.lfStrikeOut = 0;
    font.lfCharSet = RUSSIAN_CHARSET;
    font.lfOutPrecision = 1;
    font.lfClipPrecision = 1;
    font.lfQuality = 1;
    font.lfPitchAndFamily = 1;
    return CreateFontIndirect(&font);
}

void aboutMe(){
    HDC hdc = GetDC(GetConsoleWindow());
	initLeters();
    HFONT font = getFont();
    SelectObject(hdc,font);
    for (int i = 0; i < leters.size(); ++i) {
        TextOutA(hdc, 100 +leters[i] * 30, 100, name1.c_str() + leters[i], 1);
        TextOutA(hdc, 100 + leters[i] * 30, 200, name2.c_str() + leters[i], 1);
        EndPath(hdc);
        Sleep(100);
    }
    DeleteObject(font);
    ReleaseDC(GetConsoleWindow(), hdc);
}