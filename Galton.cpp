#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <tchar.h>
#include <xstring>
#include "iostream" 
#include "stdio.h" 
#include "math.h" 
#include "conio.h" 
#include <Windows.h> 
#include <random> 
using namespace std;
size_t Width, Height;
int R = 200, xr = 500, yr = 300, r = 15, x, y; //R,xr,yr - ������ � ���������� ����� ����������; r-������ ���������� �����
int GetRandomNumber(int min, int max)
{
    // ���������� ��������� ��������� �����
    srand(time(NULL));
    // �������� ��������� ����� - �������
    int num = min + rand() % (max - min + 1);
    return num;
}
void gen_cv(int* r1, int* r2, int* r3)//��������� ��������� ����� ��� �������� ���������� �����
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<>dist(0, 0);
    *r1 = dist(gen);
    *r2 = dist(gen);
    *r3 = dist(gen);
}
void cls(HDC h) {
    RECT rr; rr.left = 0; rr.top = 0; rr.bottom = 362; rr.right = 400;//���������� ��������
    HBRUSH rc = CreateSolidBrush(RGB(0, 0, 0));//���� �����
    FillRect(h, &rr, rc);//���������
}
void circle(HDC h, int r, int x, int y, COLORREF color)//�-� ��� ��������� ���������� �����
{
    HBRUSH hbr = CreateSolidBrush(color);//���� �����
    int x1, x2, y1, y2;//���������� ����� ��������, ���������� � ����������
    x1 = x - r;
    y1 = y - r;
    x2 = x + r;
    y2 = y + r;
    SelectObject(h, hbr);
    Ellipse(h, x1, y1, x2, y2);//��������� �����
}
void Circle(HDC h, int R, int X, int Y)//�-� ��� ��������� ����������(�����)
{
    HBRUSH hbr = CreateSolidBrush(RGB(225, 225, 225));
    int X1=255, X2=245, Y1=245, Y2=255;
    SelectObject(h, hbr);
    Ellipse(h, X1, Y1, X2, Y2);
    int X3 = 305, X4 = 295, Y3 = 245, Y4 = 255;
    SelectObject(h, hbr);
    Ellipse(h, X3, Y3, X4, Y4);
    int X5 = 355, X6 = 345, Y5 = 245, Y6 = 255;    
    SelectObject(h, hbr);
    Ellipse(h, X5, Y5, X6, Y6);
    int X7 = 280, X8 = 270, Y7 = 195, Y8 = 205;
    SelectObject(h, hbr);
    Ellipse(h, X7, Y7, X8, Y8);
    int X9 = 330, X10 = 320, Y9 = 195, Y10 = 205;
    SelectObject(h, hbr);
    Ellipse(h, X9, Y9, X10, Y10);
    int X11 = 295, X12 = 305, Y11 = 145, Y12 = 155;
    SelectObject(h, hbr);
    Ellipse(h, X11, Y11, X12, Y12);
}
void Line(HDC h, int R, int X, int Y)//�-� ��� ��������� ����������(�����)
{
    POINT op;
    HBRUSH hbr = CreateSolidBrush(RGB(225, 225, 225));
    MoveToEx(h, 200, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 200, 300); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 250, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 250, 300); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 300, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 300, 300); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 350, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 350, 300); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 400, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 400, 300); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 200, 400, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 400, 400); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 200, 300, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 260, 100); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 400, 300, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 340, 100); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 260, 100, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 260, 50); //������ ����� �� ������� ����� � ����� 100, 200
    MoveToEx(h, 340, 100, &op); //������ ������� ����� � ���������� 50,50
    LineTo(h, 340, 50); //������ ����� �� ������� ����� � ����� 100, 200
    SelectObject(h, hbr);

}
void dvig(HDC h, HDC hMemDC, int R, int x1, int y1, COLORREF color)//�-� ��� �������� ���������� �����
{
    double fi;
    double x = x1 = 300, y = y1 = 100, h1 = 0.1;
    double x2 = 360;
    double x3 = 0;
    int number;
    int count = 0, dx = 1;
    {
        for (int i = y; count < 40; i += dx)//���� �� 0 �� 360 ��������
        {
            fi = x3 * 3.14 / 180;//������� �������� � �������
            x = x;//��������� ���������� �����
            y = y + 0.5;
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ����  
        }
        number = GetRandomNumber(1, 2);
        for (int i = y; count < 90; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            if (number == 2)
            {
                x = x + 0.5;//��������� ���������� �����
                y = y + 0.5;
            }
            else
            {
                x = x - 0.5;
                y = y + 0.5;
            }
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ���� 
        }
        for (int i = y; count < 140; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            x = x;//��������� ���������� �����
            y = y + 0.5;
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ����  
        }
        number = GetRandomNumber(1, 2);
        for (int i = y; count < 190; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            if (number == 2)
            {
                x = x + 0.5;//��������� ���������� �����
                y = y + 0.5;
            }
            else
            {
                x = x - 0.5;
                y = y + 0.5;
            }
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ���� 
        }
        for (int i = y; count < 240; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            x = x;//��������� ���������� �����
            y = y + 0.5;
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ����  
        }
        number = GetRandomNumber(1, 2);
        for (int i = y; count < 290; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            if (number == 2)
            {
                x = x + 0.5;//��������� ���������� �����
                y = y + 0.5;
            }
            else
            {
                x = x - 0.5;
                y = y + 0.5;
            }
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ���� 
        }
        for (int i = y; count < 550; i += dx)//���� �� 0 �� 360 ��������
        {
            if (_kbhit()) break;
            fi = x3 * 3.14 / 180;//������� �������� � �������
            x = x;//��������� ���������� �����
            y = y + 0.5;
            cls(hMemDC);
            Circle(hMemDC, R, xr, yr);//��������� ����������
            Line(hMemDC, R, xr, yr);
            int r1, r2, r3;
            gen_cv(&r1, &r2, &r3);//��������� ����� �����
            circle(hMemDC, r, x, y, color);
            BitBlt(h, 0, 0, Width, Height, hMemDC, 0, 0, SRCCOPY);
            Sleep(10);//�������� � 10 ��
            x3 = x3 + h1;//����������� ���
            count++;// ������� ����
            
        
        }
        
    } 
}
void Galton(COLORREF color) {
        
        HDC hDC = GetDC(GetConsoleWindow());//����������� ��������� ������� ��� ���������
        HDC hMemDC = CreateCompatibleDC(hDC);
        Width = GetDeviceCaps(hDC, HORZRES);
        Height = GetDeviceCaps(hDC, VERTRES);
        HBITMAP hMemBm = CreateCompatibleBitmap(hDC, Width, Height);
        SelectObject(hMemDC, hMemBm);
        HPEN Pen = CreatePen(PS_SOLID, 2, color);
        SelectObject(hMemDC, Pen);
        int i = 0;
        while (i < 4) {
            dvig(hDC, hMemDC, R, x, y, RGB(0, 0, 225));
            i++;
        }
        DeleteObject(Pen);
        DeleteObject(hMemBm);
        DeleteDC(hMemDC);
        DeleteDC(hDC);
    
    
}

