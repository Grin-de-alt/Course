#include <windows.h>
#include <string>
#include "Keyboard.h"// 
#include "Menu.h"


void setTextSettings(HDC hdc,COLORREF BkCollor= RGB(0, 0, 0), COLORREF TextCollor= RGB(255, 255, 255),
    int Height = stdTextHeight,int Width = 0,int Weight = 100,bool Italic = 0,bool Underline = 0,bool StrikeOut = 0) {
    LOGFONT font;
    font.lfHeight = Height;
    font.lfWidth = Width;
    font.lfEscapement = 0;
    font.lfOrientation = 0;
    font.lfWeight = Weight;
    font.lfItalic = Italic;
    font.lfUnderline = Underline;
    font.lfStrikeOut = StrikeOut;
    font.lfCharSet = RUSSIAN_CHARSET;
    font.lfOutPrecision = 0;
    font.lfClipPrecision = 0;
    font.lfQuality = 0;
    font.lfPitchAndFamily = 0;

    SelectObject(hdc, CreateFontIndirect(&font));
    SetBkColor(hdc, BkCollor);
    SetTextColor(hdc, TextCollor);
}



void PrintMenu(HDC hdc,std::string* items,int count,int x,int y){
	for (int i = 0; i < count; ++i) {
		TextOutA(hdc, x, y, items[i].c_str(), items[i].size()); 
		y += stdTextHeight;
	}
	EndPath(hdc);
}

int ShowMenu(std::string* items, int count){
	HDC hdc = GetDC(GetConsoleWindow()); 
    setTextSettings(hdc);
	PrintMenu(hdc,items,count,100,100);
    Keys pressedKey; 
    int choise = 0; 
    TextOutA(hdc, 80, 100, ">", 1); 
    EndPath(hdc);
    while ((pressedKey = GetKey()) != Enter){ 
        setTextSettings(hdc, RGB(0, 0, 0), RGB(0, 0, 0)); 
        TextOutA(hdc, 80, 100 + choise * 20, ">", 1); 
        switch (pressedKey) {
       
        case Up:
            choise = (choise + count - 1) % count; 
            break;
        case Down:
            choise = (choise + 1) % count;
            break;
        }
        setTextSettings(hdc, RGB(0, 0, 0), RGB(255, 255, 255)); 
        TextOutA(hdc, 80, 100 + choise * 20, ">", 1);
        EndPath(hdc);
    }
    ReleaseDC(GetConsoleWindow(), hdc);
	return choise; 
}